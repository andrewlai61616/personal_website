import RealPrices as RP # ~6:10
import numpy as np
import tensorflow as tf

env = RP.forex_env()

# Implementing the network itself
tf.reset_default_graph()

#These lines establish the feed-forward part of the network used to choose actions
X = tf.placeholder(shape=[1,1049],dtype=tf.float32)

w1 = tf.Variable(tf.random_normal(shape=(1049,500)))
b1 = tf.Variable(tf.random_normal(shape=(1,500)))

w2 = tf.Variable(tf.random_normal(shape=(500,200)))
b2 = tf.Variable(tf.random_normal(shape=(1,200)))

w3 = tf.Variable(tf.random_normal(shape=(200,100)))
b3 = tf.Variable(tf.random_normal(shape=(1,100)))

w4 = tf.Variable(tf.random_normal(shape=(100,50)))
b4 = tf.Variable(tf.random_normal(shape=(1,50)))

w5 = tf.Variable(tf.random_normal(shape=(50,21)))
b5 = tf.Variable(tf.random_normal(shape=(1,21)))

# structure of MLP
hidden_layer_1 = tf.nn.relu(tf.matmul(X, w1) + b1)
hidden_layer_2 = tf.nn.relu(tf.matmul(hidden_layer_1, w2) + b2)
hidden_layer_3 = tf.nn.relu(tf.matmul(hidden_layer_2, w3) + b3)
hidden_layer_4 = tf.nn.relu(tf.matmul(hidden_layer_3, w4) + b4)
Qout = tf.sigmoid(tf.matmul(hidden_layer_4, w5) + b5)
Qout_2d = tf.reshape( Qout, [7,3] )
predict = tf.argmax(Qout_2d, 1)

#Below we obtain the loss by taking the sum of squares difference between the target and prediction Q values.
nextQ = tf.placeholder(shape=[1,21],dtype=tf.float32)

loss = tf.reduce_sum(tf.square(nextQ - Qout))

trainer = tf.train.GradientDescentOptimizer(learning_rate=0.04)
updateModel = trainer.minimize(loss)

#Training the network
init = tf.global_variables_initializer()

# Set learning parameters
y = .99
e = 0.04
num_steps = 10000000 # there is only one episode
good_start_check_i = 1000

tf.device('/GPU:0')
with tf.Session() as sess:
	
	training_done = False
	while not training_done:
		sess.run(init)
		s = env.reset()
		sumR = np.array([0])
		env.now_bid_ask_diff = env.start_bid_ask_diff
		
		for i in range(num_steps):
			# make let bid_ask_diff = 0.00002 -> 0.0002 as i increases
			if (i+1) % 50000 == 0 and env.now_bid_ask_diff < env.target_bid_ask_diff:
				env.now_bid_ask_diff += env.step_bid_ask_diff
			
			#Choose an action by greedily (with e chance of random action) from the Q-network
			a, allQ = sess.run([predict, Qout_2d],feed_dict={X:s})
			real_r = env.reward( env.t, a )
			if np.random.rand(1) < e:
				a = env.random_action_sample()
			#Get new state and reward from environment
			s1, r = env.step(a)
			#Obtain the Q' values by feeding the new state through our network
			Q1 = sess.run(Qout_2d,feed_dict={X:s1})
			#Obtain maxQ' and set our target value for chosen action.
			maxQ1 = np.max(Q1, axis = 1)
			targetQ = allQ
			for Qi in range(len(Q1)):
				targetQ[Qi][a[Qi]] = r + y*maxQ1[Qi]
			targetQ = np.reshape( targetQ, (1,21) )
			sumR = np.append(sumR, sumR[-1]+real_r)
			
			# sumR should always be non-negative
			if sumR[-1] < 0:
				break
			
			#Train our network using target and predicted Q values
			sess.run([updateModel,w1,b1,w2,b2,w3,b3,w4,b4,w5,b5],feed_dict={X:s,nextQ:targetQ})
			s = s1
			
			if i > 100 and i % 1000 == 0: #int(num_steps / 100) == 0: # int(num_steps / 100)
				print("step " + '%d' % i + " sectR = " + '%.4f' % (sumR[-1] - sumR[-1000]) + \
			 " total sumR = " + '%.4f' % (sumR[-1]) + " bidaskdiff = " + '%.5f' % env.now_bid_ask_diff)
			if i == num_steps - 1:
				training_done = True
				break
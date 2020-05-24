import VAR
import Get_On_Browser
import Visit_Web
import os, time, getopt, datetime
from bs4 import BeautifulSoup

def check_1_day_restart_web( password ):
	if datetime.datetime.today() > VAR.last_web_open_time + datetime.timedelta(days = 1):
		print("One day passed. Web restarting...")
		VAR.web = None
		Visit_Web.visit( demo_account = VAR.demo_account, password = password ) # Visit webpage

from scp import SCPClient
import paramiko
def get_predict():
		client = paramiko.SSHClient()
		client.load_system_host_keys()
		client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
		print('\tGetting prediction value from 36.227.33.26')
		client.connect('36.227.33.26', 22, 'godlai', VAR.ex_params['str_get']) #local '192.168.1.5'
		scp = SCPClient(client.get_transport())
		scp.get('/home/godlai/Documents/forex/predict.txt', 'predict.txt')
		scp.close()
		client.close()
		f = open('predict.txt', 'r')
		t, predict = f.read().split(',')
		f.close()
		t = datetime.datetime.strptime(t,'%Y-%m-%d %H:%M:%S')
		predict = float(predict)
		VAR.predict_t, VAR.predict_rate = t, predict

def print_status():
	now = datetime.datetime.today()
	now = now.replace(microsecond = 0)
	print(now, "\n" + "{:>18}".format("Total=" + str(VAR.total_amount)) + 
		"{:>18}".format("\tBought=" + str(VAR.bought_amount)) + 
		"{:>18}".format("\tUsable=" + str(VAR.usable_amount)) +
		"{:>18}".format("\tProfit=" + str(VAR.current_total_profit)) )
	print( "\tPredict time=" + str(VAR.predict_t) + 
		"{:>18}".format("rate=" + str(VAR.predict_rate)) )
	print( "\tLast trade time:", VAR.last_trade_time )
	print("\tInvestments:\n")
	for item in VAR.invest:
		print(item)

def import_external_parameters():
	# Import external parameters
	VAR.ex_params = {}
	with open ('external_parameters.csv', 'r') as fp:
		line = fp.readline() # 第一行為 標題
		line = fp.readline()
		while line:
			values = line.split(',')
			VAR.ex_params[values[0]] = values[1]
			line = fp.readline()
	VAR.ex_params['str_get'] = VAR.ex_params['str_get'].replace('\'','')
	VAR.threshold = float(VAR.ex_params['threshold'])
	VAR.rate_per_buy = float(VAR.ex_params['rate_per_buy'])
	VAR.currency = VAR.ex_params['currency']
	VAR.multiplier = int(VAR.ex_params['multiplier'])
	VAR.permit_weekday_end = int(VAR.ex_params['permit_weekday_end'])
	VAR.permit_weekday_start = int(VAR.ex_params['permit_weekday_start'])
	VAR.permit_hour_end = int(VAR.ex_params['permit_hour_end'])
	VAR.permit_hour_start = int(VAR.ex_params['permit_hour_start'])
	VAR.commission_rate = float(VAR.ex_params['commission_rate'])
	VAR.wait_period = int(VAR.ex_params['wait_period'])
      
def get_arguments(argv):
	VAR.demo_account = False
	try:
		opts, args = getopt.getopt(argv,"o:")
		for opt, arg in opts:
			if opt == '-o':
				if str(arg).replace(' ','') == 'demo':
					VAR.demo_account = True
					print("Using demo account")
	except getopt.GetoptError:
		print("ERROR getting parameters! Neglecting all.")
		pass
    
def loop_weekend():
	tm_wday = time.gmtime().tm_wday
	tm_hour = time.gmtime().tm_hour
	while((tm_wday == 4 and tm_hour >= 21) or tm_wday == 5 or (tm_wday == 6 and tm_hour < 21)):
		os.system('cls')
		print("\n\n\nLibertex is currently not available (weekends).\nCurrent time is: " + time.ctime() + " (GMT+8)")
		print("Libertex will start working at Monday 05:00 (GMT+8) and so will Main_System.py. Good luck!")
		print("\n\nWaiting 1 minute...")
		time.sleep(60)
		tm_wday = time.gmtime().tm_wday
		tm_hour = time.gmtime().tm_hour

def Check_Weekend():
	tm_wday = time.gmtime().tm_wday
	tm_hour = time.gmtime().tm_hour
	if((tm_wday == 4 and tm_hour >= 21) or tm_wday == 5 or (tm_wday == 6 and tm_hour < 21)):
		loop_weekend()
		print("HEY! Remember to try to close VAR.web and open up a new browser")
		while True:
			a = 1

def Web_Update():
	VAR.soup = BeautifulSoup(VAR.web.page_source, 'lxml')
	# Get_On_Browser.get_currency_list()
	Get_On_Browser.get_usable_and_bought()
	Get_On_Browser.update_user_invest_list()
	# Get_On_Browser.update_buy_lists()
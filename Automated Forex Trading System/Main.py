import sys, time # 系統需求

import os
os.chdir('C:\\Users\\User\\Desktop\\Python_Projects\\Libertex_New')
import VAR
import Visit_Web
import Fnc_Main
import INFO_SAVE
import Transactions

def main(argv):
	VAR.init() #initialize VAR and its variables
	if len(argv)>=2:
		Fnc_Main.get_arguments(argv[1:]) # get command line arguments
	else:
		VAR.demo_account = True
	Fnc_Main.import_external_parameters() # Import external parameters
	Visit_Web.visit( demo_account = VAR.demo_account, password = argv[0] ) # Visit webpage
	INFO_SAVE.save_record("LOGIN","",0,0, "LOGIN SUCCESS") # Just a login record
  
	while True: # Main loop's here
		Fnc_Main.check_1_day_restart_web( password = argv[0] )
		Fnc_Main.get_predict()
		Fnc_Main.Web_Update()
		''' The variables should then be loaded
		VAR. bought_amount , usable_amount , current_total_profit ,
		total_amount , predict_t , predict_rate'''
		Fnc_Main.print_status()
		Transactions.decision()
		time.sleep(5)

if __name__ == "__main__":
	if len(sys.argv)==1:
		print("Please enter password")
	else:
		main(sys.argv[1:])
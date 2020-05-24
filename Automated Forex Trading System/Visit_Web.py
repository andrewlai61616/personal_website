import VAR
import time, datetime
from selenium import webdriver # 瀏覽網站用
from selenium.webdriver.common.keys import Keys # 在網站鍵盤輸入用

def visit( demo_account = False, password = "" ):
	VAR.web = webdriver.Chrome(chrome_options=VAR.chrome_options)  # Open incog tab browser
	print("Visiting Webpage ...")
	VAR.web.get(VAR.ex_params['url'])
	VAR.web.maximize_window()
  
	VAR.web.find_element_by_name("login").send_keys( VAR.ex_params['email'] )
	VAR.web.find_element_by_name("password").send_keys( password + Keys.ENTER )
  
	# 切換到 none grouping
	while VAR.web.find_elements_by_name("productsGroup") == []:
		time.sleep(0.5)
	none_radio = VAR.web.find_elements_by_name("productsGroup")[1]
	action = webdriver.common.action_chains.ActionChains(VAR.web)
	action.move_to_element_with_offset(none_radio, 5, 5)
	action.click()
	action.perform()
	time.sleep(0.5)
  
	print("\tWaiting for log in ...")
	while VAR.web.find_elements_by_class_name('switch-account') == []:
		time.sleep(1)
    
	if demo_account:
		# Press Account button -> switch account btn -> OK
		VAR.web.find_element_by_class_name("profile-nav-inner").click()
		while VAR.web.find_elements_by_class_name('switch-account') == []:
			time.sleep(0.5)
		time.sleep(0.3)
		VAR.web.find_element_by_class_name("switch-account").click()
		while VAR.web.find_elements_by_class_name('modal-switch-account') == []:
			time.sleep(0.5)
		time.sleep(0.3)
		VAR.web.find_element_by_class_name("modal-switch-account").find_element_by_class_name("ok").click()

		print("\tWaiting for switching to demo account ...")
		while VAR.web.find_elements_by_class_name('change-demo-balance') == []:
			time.sleep(1)
    
	VAR.web.get(VAR.ex_params['url_active'])
	VAR.last_web_open_time = datetime.datetime.today()
	print("---Target webpage achieved---")
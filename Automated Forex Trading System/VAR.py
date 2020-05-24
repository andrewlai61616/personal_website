global ex_params                    # Saving external parameters defined by the user
global web                          # Selenium web browsing
global demo_account                 # Use demo account or not
global soup                         # Parse website
global currency_list                # The list of available currencies to buy
global day_shift                    # The list of daily currency shift
global month_shift                  # The list of monthly currency shift
global total_amount
global bought_amount
global usable_amount
global current_total_profit
global last_web_open_time
global last_trade_time
global invest

global predict_t
global predict_rate

global threshold
global currency
global rate_per_buy
global multiplier
global permit_weekday_end
global permit_weekday_start
global permit_hour_end
global permit_hour_start
global commission_rate
global wait_period

from selenium import webdriver # 瀏覽網站用
chrome_options = webdriver.ChromeOptions()
chrome_options.add_argument("--incognito")

import datetime

def init():
	global demo_account
	global last_trade_time
	last_trade_time = datetime.datetime.today() - datetime.timedelta(hours = 5)
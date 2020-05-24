import VAR
import INFO_SAVE
import time, datetime
from bs4 import BeautifulSoup

def get_currency_list():
  VAR.currency_list = {} # global
  VAR.month_shift = {} # global
  VAR.day_shift = {} # global
  products_list = []
  try_soup = 0
  while products_list == []:
    #print('Getting currency list...')
    try:
      products_list = VAR.soup.find("div", {"class": "products-list"}) \
                            .find_all("div", {"class": "row"})
    except Exception as e:
      try_soup = try_soup + 1
      if try_soup == 10:
        print('Cannot get soup while getting currency list. Tried 10 times!')
        INFO_SAVE.save_record("ERROR","SHUTDOWN",0, 0, "Cannot get soup while getting currency list. Tried 10 times!")
        VAR.soup.find("div", {"class": "products-list"})
        
      print('ERROR getting products list! Trying again')
      VAR.web.get( VAR.ex_params['url'] )
      time.sleep(4)
      VAR.soup = BeautifulSoup( VAR.web.page_source, 'lxml' )
      time.sleep(0.5)
      pass
      
  for item in products_list:
    col = item.find("a", {"class":"alias"}).text
    rate = item.find("div", {"class":"col col-rate"}).text
    rate = rate.replace(" ","").replace("\xa0","")
    VAR.currency_list[col] = float(rate)
    
    # Get daily incline decline
    profit = item.find("div", {"class":"col col-profitDay"}).text
    profit = profit.replace(" ","").replace("\xa0","").replace("%","")
    VAR.day_shift[col] = float(profit)*0.01
    
    # Get monthly incline decline
    profit = item.find("div", {"class":"col col-profitMonth"}).text
    profit = profit.replace(" ","").replace("\xa0","").replace("%","")
    VAR.month_shift[col] = float(profit)*0.01
    
    ### Consider getting half-year and yeay shift

def get_usable_and_bought():
  spoiler = VAR.soup.find("div",{"class":"spoiler"})
  VAR.bought_amount = spoiler.find("dd",{"class":"margin"}).text
  VAR.usable_amount = spoiler.find("dd",{"class":"spareCash"}).text
  VAR.current_total_profit = spoiler.find("dd",{"class":"profit"}).text
  
  VAR.bought_amount = float(VAR.bought_amount.replace(" ","").replace("\xa0",""))
  VAR.usable_amount = float(VAR.usable_amount.replace(" ","").replace("\xa0",""))
  VAR.current_total_profit = float(VAR.current_total_profit.replace(" ","").replace("\xa0",""))
  VAR.total_amount = VAR.bought_amount + VAR.usable_amount
  
def update_user_invest_list():
  # assume already at url_active
  VAR.invest = []
  while True:
    try:
      invest_list = VAR.web.find_element_by_class_name("investments-list")
      invest_list = invest_list.find_elements_by_class_name("row")
      break
    except Exception as e:
      time.sleep(0.5)
      pass
  for item in invest_list:
    item_dict = {}
    item_dict['startTime'] = item.find_element_by_class_name("col-startTime").text.replace('月','-').replace('日','')
    item_dict['startTime'] = datetime.datetime.strptime(item_dict['startTime'], '%m-%d %H:%M:%S').replace(year=datetime.date.today().year)
    item_dict['startTime'] = item_dict['startTime'] + datetime.timedelta(hours = 8)
    item_dict['startRate'] = float(item.find_element_by_class_name("col-startRate").text)
    item_dict['tradeAmount'] = float(item.find_element_by_class_name("col-sumInv").text.replace('$',''))
    item_dict['nowAmount'] = float(item.find_element_by_class_name("col-equityInv").text.replace('$',''))
    VAR.invest.append(item_dict)

def update_buy_lists():
  update_user_invest_list()
  #print("Done getting user invest list")
  
  delete_list = []
  for item in VAR.new_buy_list.keys():
    if item not in VAR.user_invest_list.keys():
      INFO_SAVE.save_record("SOLD",item,VAR.total_amount + VAR.current_total_profit,VAR.usable_amount, "")
      delete_list.append(item)
  
  for item in delete_list:
    del VAR.new_buy_list[item]
      
  delete_list = []
  for item in VAR.save_list.keys():
    if item not in VAR.user_invest_list.keys():
      INFO_SAVE.save_record("SOLD",item,VAR.total_amount + VAR.current_total_profit,VAR.usable_amount, "")
      delete_list.append(item)
  
  for item in delete_list:
    del VAR.save_list[item]
      
  delete_list = []
  for item in VAR.eager_save_list.keys():
    if item not in VAR.user_invest_list.keys():
      INFO_SAVE.save_record("SOLD",item,VAR.total_amount + VAR.current_total_profit,VAR.usable_amount, "")
      VAR.sent_mayday_email[item] = False
      delete_list.append(item)
  
  for item in delete_list:
    del VAR.eager_save_list[item]
    
  '''if there appears to be any manually added items save them to list'''
  for item in VAR.user_invest_list.keys():
    if VAR.user_invest_list[item][1] < VAR.total_amount*VAR.ex_params['rate_per_buy']*(1+VAR.ex_params['save_buy_ratio']/2):
      if item not in VAR.new_buy_list.keys():
        VAR.new_buy_list[item] = 0
        print("\tFound manually added new buy: " + item)
    elif VAR.user_invest_list[item][1] < VAR.total_amount*VAR.ex_params['rate_per_buy']*(1+VAR.ex_params['save_buy_ratio']/2)*(1+VAR.ex_params['save_buy_ratio']/2):
      if item not in VAR.save_list.keys():
        VAR.save_list[item] = 0
        if item in VAR.new_buy_list.keys():
          del VAR.new_buy_list[item]
        print("\tFound manually added save: " + item)
    else:
      if item not in VAR.eager_save_list.keys():
        VAR.eager_save_list[item] = 0
        if item in VAR.save_list.keys():
          del VAR.save_list[item]
        print("\tFound manually added eager save: " + item)

def get_possible_currency_list(currency_list, often_use, day_incline_decline, per_day_max_change, per_day_min_change, month_incline_decline, per_month_max_change, per_month_min_change, cfee_no_check_loop, new_buy_list, save_list, eager_save_list):
  possible_currency_list = {}
  really_possible_list = {}
  for item in currency_list.keys():
    if item[:3] in often_use and item[4:] in often_use:
      if per_day_min_change < day_incline_decline[item] and (day_incline_decline[item] < per_day_max_change or day_incline_decline[item]+(month_incline_decline[item]/3) < 0 ):
        if per_month_min_change < month_incline_decline[item] and month_incline_decline[item] < per_month_max_change:
          if item not in new_buy_list.keys() and item not in save_list.keys() and item not in eager_save_list.keys():
            possible_currency_list[item] = currency_list[item]       
            if item not in cfee_no_check_loop.keys():
              really_possible_list[item] = currency_list[item]
  return possible_currency_list, really_possible_list
from selenium.webdriver.common.keys import Keys

import VAR
import INFO_SAVE
import datetime, time

def trade(buy = False, sell = False):
  if not buy and not sell:
    return
  trade_amount = int(VAR.total_amount * VAR.rate_per_buy)
  VAR.web.get(VAR.ex_params['url'] + "#modal_newInvest_" + VAR.currency)
              
  # Enter correct values and trade
  while True:
    try:
      sumInv = VAR.web.find_element_by_name("sumInv")
      sumInv.send_keys(Keys.CONTROL, 'a')
      sumInv.send_keys(str(trade_amount))
      
      mult = VAR.web.find_element_by_name("mult")
      mult.send_keys(Keys.CONTROL, 'a')
      mult.send_keys(str(VAR.multiplier))
      
      if buy:
        buy_button = VAR.web.find_element_by_class_name("a-growth")
        time.sleep(0.3)
        buy_button.click()
      elif sell:
        sell_button = VAR.web.find_element_by_class_name("a-reduction")
        time.sleep(0.3)
        sell_button.click()
      break
    except Exception as e:
      #print(e)
      #print("Waiting 0.5sec...")
      time.sleep(0.5)
      pass
  # wait for next page to show
  while True:
    try:
      VAR.web.find_element_by_class_name("box-row-text").find_elements_by_css_selector("p")[1].text
      break
    except Exception as e:
      #print(e)
      #print("Waiting 0.3sec...")
      time.sleep(0.3)
      pass
  # goto active tradements
  time.sleep(0.5)
  print("\tSetting limit values")
  VAR.web.get(VAR.ex_params['url_active'])
  while True:
    try:
      invest_list = VAR.web.find_element_by_class_name("investments-list")
      time_list = invest_list.find_elements_by_class_name("col-startTime")
      break
    except Exception as e:
      time.sleep(0.5)
      pass
  # click on the last opened trade
  time_t = datetime.datetime.today() - datetime.timedelta(hours = 8, minutes = 5)
  x = -1
  for id in range(len(time_list)):
    trade_time = time_list[id].text.replace('月','-').replace('日','')
    trade_time = datetime.datetime.strptime(trade_time, '%m-%d %H:%M:%S')
    trade_time = trade_time.replace(year=datetime.date.today().year)
    if time_t < trade_time:
      time_t = trade_time
      x = id
  invest_list = invest_list.find_elements_by_class_name('row')
  invest_list[x].click()
  # press the set limit button
  try:
    VAR.web.find_element_by_id("limitUnitUsd").send_keys(Keys.SPACE)
  except Exception as e:
    while True:
      try:
        limit_link = VAR.web.find_element_by_class_name("limit-link")
        limit_link.click()
        break
      except Exception as e:
        time.sleep(0.5)
        pass
    while True:
      try:
        limit_link = VAR.web.find_element_by_id("limitUnitUsd")
        limit_link.send_keys(Keys.SPACE)
        break
      except Exception as e:
        time.sleep(0.5)
        pass
    pass
  # set upper limit
  while True:
    try:
      limit_link = VAR.web.find_element_by_name("haslimitTP")
      limit_link.send_keys(Keys.SPACE)
      break
    except Exception as e:
      time.sleep(0.5)
      pass
  trade_profit = trade_amount * float(VAR.ex_params['target_profit_rate'])
  mod_trade_profit = round(trade_profit - VAR.commission_rate * VAR.multiplier * trade_amount)
  while True:
    try:
      limit_link = VAR.web.find_element_by_name("limitTP")
      limit_link.send_keys(Keys.CONTROL + 'a')
      limit_link.send_keys(str(mod_trade_profit))
      break
    except Exception as e:
      time.sleep(0.5)
      pass
  while True:
    try:
      limit_link = VAR.web.find_element_by_name("haslimitSL")
      limit_link.send_keys(Keys.SPACE)
      break
    except Exception as e:
      time.sleep(0.5)
      pass
  stop_loss = trade_profit / float(VAR.ex_params['stop_loss_ratio'])
  mod_stop_loss = round(stop_loss + VAR.commission_rate * VAR.multiplier * trade_amount)
  while True:
    try:
      limit_link = VAR.web.find_element_by_name("limitSL")
      limit_link.send_keys(Keys.CONTROL + 'a')
      limit_link.send_keys(str(mod_stop_loss))
      break
    except Exception as e:
      time.sleep(0.5)
      pass
  # save limits and go back to url
  while True:
    try:
      save_link = VAR.web.find_element_by_class_name("limits-save")
      save_link.click()
      break
    except Exception as e:
      time.sleep(0.5)
      pass
  VAR.web.get(VAR.ex_params['url_active'])
  if buy:
    save_record_title = 'BUY'
  else:
    save_record_title = 'SELL'
  INFO_SAVE.save_record(save_record_title, VAR.ex_params['currency'],
    VAR.total_amount, trade_amount, "stage_1 strategy 2018data")
  VAR.last_trade_time = datetime.datetime.today()

def end_trade( id, nowAmount ):
  while True:
    try:
      invest_list = VAR.web.find_element_by_class_name("investments-list")
      invest_list = invest_list.find_elements_by_class_name("row")
      # check if the time really is right
      now = datetime.datetime.today()
      startTime = invest_list[id].find_element_by_class_name("col-startTime").text.replace('月','-').replace('日','')
      startTime = datetime.datetime.strptime(startTime, '%m-%d %H:%M:%S').replace(year=now.year)
      startTime = startTime + datetime.timedelta(hours = 8)
      if startTime + datetime.timedelta(minutes = VAR.wait_period) >= now:
        return
      # press close trade button
      close_link = invest_list[id].find_element_by_class_name("invest-close")
      close_link.click()
      break
    except Exception as e:
      time.sleep(0.5)
      pass
  while True:
    try:
      # press close trade button
      close_link = VAR.web.find_element_by_class_name("invest-close-ok")
      close_link.click()
      break
    except Exception as e:
      time.sleep(0.5)
      pass
  INFO_SAVE.save_record('CLOSE', VAR.ex_params['currency'],
    VAR.total_amount, nowAmount, "stage_1 strategy 2018data")
  VAR.web.get(VAR.ex_params['url_active'])

def decision():
  # 如果有一筆交易「超過三個小時」且「有營利」那不管怎樣就把他結束掉
  now = datetime.datetime.today()
  for id in range(len(VAR.invest)):
    if VAR.invest[id]['startTime'] + datetime.timedelta(minutes = VAR.wait_period) < now:
      if VAR.invest[id]['nowAmount'] > VAR.invest[id]['tradeAmount']:
        end_trade(id, VAR.invest[id]['nowAmount'])
        break
  # 簡單說如果超過 threshold , 時間在當下或前一分鐘 , 錢夠 -> 交易
  now = datetime.datetime.today()
  if now > VAR.last_trade_time + datetime.timedelta(minutes = 30) and \
      now <= VAR.predict_t + datetime.timedelta(minutes=1) and \
      VAR.total_amount * VAR.rate_per_buy < VAR.usable_amount and \
      now.weekday() <= VAR.permit_weekday_end and \
      now.weekday() >= VAR.permit_weekday_start and \
      now.hour <= VAR.permit_hour_end and \
      now.hour >= VAR.permit_hour_start:
    if VAR.predict_rate > VAR.threshold + 0.5:
      print("\tBuy!")
      trade(buy = True)
    elif VAR.predict_rate < 0.5 - VAR.threshold:
      print("\tSell!")
      trade(sell = True)
    else:
      print("\tDon't Trade!")
  else:
    print("\tDon't Trade!")
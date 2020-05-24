'''
  A record is of the format:
  [YYYYMMDD] \t [HHMMSS] \t [Buy/Sold] \t [Currency] \t [Total] \t [set_profit]
'''

import datetime
import VAR

def save_record(title, currency, total, set_profit, comments):
  if VAR.demo_account == True:
    demo = "_Demo"
  else:
    demo = "_Real"
  with open("Libertex_Data" + demo + ".txt", "a") as dat:
    dat.write( datetime.datetime.strftime(datetime.datetime.today(), "%Y-%m-%d %H:%M:%S") + "," + title + "," + currency + "," + \
      str(format(total, '.8g')) + "," + str(format(set_profit, '.8g')) + "," + comments + "\n")
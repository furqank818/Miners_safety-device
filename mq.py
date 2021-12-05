import smtplib, ssl
import json
import sys
import time
import RPi.GPIO as GPIO
port = 465  # For SSL
smtp_server = "smtp.gmail.com"
sender_email = "your email address"  # Enter your address
receiver_email = "reciever address"  # Enter receiver address
password = "your email password"
message = """\
Subject: Alert

There has been Alert for mines condition please check dashboard and take action"""
GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
GPIO.setup(21,GPIO.OUT)
GPIO.setup(23,GPIO.OUT)


def measure():
    try:
       
        if(temrdata > "38") or (humidata >"80" ) or(airdata > "500.00"):
            
            GPIO.output(21,GPIO.HIGH)
            GPIO.output(23,GPIO.HIGH)
            print ("exhaust system on")
            
            
            context = ssl.create_default_context()
            with smtplib.SMTP_SSL(smtp_server, port, context=context) as server:
             server.login(sender_email, password)
             server.sendmail(sender_email, receiver_email, message)
        else:
              GPIO.output(21,GPIO.LOW)
              GPIO.output(23,GPIO.LOW)
              print ("Exhaust system off")
        
    except:
        print("connection failed")
 
 
if __name__ == "__main__":
    channel = thingspeak.Channel(id=channel_id, api_key=read_key,fmt='txt')
    while True:
        measure(channel)
        # free account has an api limit of 15sec
        time.sleep(15)

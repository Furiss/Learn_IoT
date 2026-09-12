```mermaid
flowchart LR
esp32["ESP32"]



iot_core["AWS IoT Core<br/>iot-course/vlasenko/data"]
iot_rule["Rules Engine"]
dynamodb["DynamoDB<br/> table IoT_Data"]
cloudwatch["CloudWatch Logs"]

esp32 -->|"MQTT publish"| iot_core
iot_core --> iot_rule
iot_rule -->|"StoreData rule"| dynamodb
iot_rule -->|"Hight_temperature rule <br/>Temperature > 28"| cloudwatch
```

Policy:
{
  "Version": "2012-10-17",
  "Statement": [
    {
      "Effect": "Allow",
      "Action": "iot:Connect",
      "Resource": "arn:aws:iot:eu-north-1:042396229421:client/ESP32_Vlasenko_Homework_5"
    },
    {
      "Effect": "Allow",
      "Action": "iot:Publish",
      "Resource": "arn:aws:iot:eu-north-1:042396229421:topic/iot-course/vlasenko/data"
    }
  ]
}

![MQTT_Test_client](screenshots/MQTTtest.PNG)
![DynamoDB](screenshots/DynamoDB.PNG)
![CloudWatch_Logs](screenshots/CloudWatch.PNG)
![CloudWatch_Opened_Log](screenshots/InsideLog.PNG)
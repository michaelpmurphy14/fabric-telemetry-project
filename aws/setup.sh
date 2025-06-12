#!/bin/bash

# Create the IoT rule
aws iot create-topic-rule \
  --rule-name ForwardToTimestream \
  --topic-rule-payload file://aws/iot/rules/forward_to_timestream.json

# Attach the IAM policy
aws iam put-role-policy \
  --role-name iot-timestream-role \
  --policy-name IoTTimestreamAccess \
  --policy-document file://aws/iam/iot_timestream_policy.json

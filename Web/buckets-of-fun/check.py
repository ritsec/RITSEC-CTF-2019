import requests
import boto3

BUCKET_NAME = 'list-s3.scriptingis.life'
FLAG_FILE = 'youfoundme-asd897kjm.txt'
FLAG = 'RITSEC{LIST_HIDDEN_FILES}'

# Check that you can make an HTTP request to list the contents of the bucket
def check_web_list(bucket_name, flag_file):
    r = requests.get('http://{}.s3.amazonaws.com/'.format(bucket_name))
    if r.status_code == 200:
        if flag_file in r.text:
            return True
        else:
            raise Exception("[WEB] Couldn't find flag filename in list contents")
    else:
        raise Exception("[WEB] Got status {} when making list request to S3 bucket".format(r.status_code))

# Check that you can make an HTTP GET request to retrieve the flag file
def check_web_get(bucket_name, flag_file, flag):
    r = requests.get('http://{}/{}'.format(bucket_name, flag_file))
    if r.status_code == 200:
        if flag in r.text:
            return True
        else:
            raise Exception("[WEB] Expected flag {} did not match file content {}".format(flag, r.text))
    else:
        raise Exception("[WEB] Got status {} when making GET request to flag file".format(r.status_code))

# Check that you can use the AWS API to list the bucket contents
def check_boto_list(bucket_name, flag_file):
    s3 = boto3.client('s3')
    files = s3.list_objects_v2(Bucket=bucket_name)
    for f in files['Contents']:
        if flag_file == f['Key']:
            return True
    
    raise Exception("[BOTO] Flag file not listed when calling list_objects_v2")

# Check that you can use the AWS API to download the flag file
def check_boto_get(bucket_name, flag_file, flag):
    s3 = boto3.resource('s3')
    bucket = s3.Bucket(bucket_name)

    get_file = s3.Object(bucket_name, flag_file)
    myfile = get_file.get()
    mytext = myfile['Body'].read().decode('utf-8')
    if flag != mytext:
        raise Exception("[BOTO] Expected flag {} did not match file content {}".format(flag, mytext))
    else:
        return True

def main():
    if check_boto_list(BUCKET_NAME, FLAG_FILE):
        print("List with boto succeeded.")
    if check_boto_get(BUCKET_NAME, FLAG_FILE, FLAG):
        print("Get with boto succeeded.")
    if check_web_list(BUCKET_NAME, FLAG_FILE):
        print("List with web succeeded.")
    if check_web_get(BUCKET_NAME, FLAG_FILE, FLAG):
        print("Get with web succeeded.")

if __name__ == "__main__":
    main()
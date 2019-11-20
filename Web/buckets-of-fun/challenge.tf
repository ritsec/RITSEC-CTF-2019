provider "aws" {
    region = "us-east-1"    
}

variable "bucket_name" {
    type = "string"
    default = "list-s3.scriptingis.life"
}

variable "flag" {
    type = "string"
    default = "RITSEC{LIST_HIDDEN_FILES}"
}

resource "aws_s3_bucket" "chal_bucket" {
    bucket = "${var.bucket_name}"
    policy = <<EOF
{
"Version": "2012-10-17",
"Id": "Basic read and list access",
"Statement": [
    {
        "Sid": "Allow listing contents",
        "Effect": "Allow",
        "Principal": "*",
        "Action": "s3:ListBucket",
        "Resource": "arn:aws:s3:::${var.bucket_name}"
    },
    {
        "Sid": "Allow public read to all files",
        "Effect": "Allow",
        "Principal": "*",
        "Action": "s3:GetObject",
        "Resource": "arn:aws:s3:::${var.bucket_name}/*"
    }
    ]
}
    EOF

    website {
        index_document = "index.html"
        error_document = "error.html"
    }

}

resource "aws_s3_bucket_object" "site_file" {
    bucket = "${aws_s3_bucket.chal_bucket.bucket}"
    key = "index.html"
    content = <<EOF
<!DOCTYPE html>
<html>
    <head>
        <title>Welcome to my challenge!</title>
    </head>
    <body>
        <h1>Welcome to the challenge!</h1>
        <form name="auth_form" method="post" action="javascript:alert('You won\'t be getting around this auth.');">
            <h3>Password</h3>
            <input type="password" name="page" autocorrect="off" autocapitalize="off" autofocus>
            <input type="submit" value="Enter">
            </div>
        </form>

        <noscript>
                <div>Javascript is required to use this site. Yours seems to be disabled.</div>
        </noscript>
    </body>
</html>
    EOF

    content_type = "text/html"
}

resource "aws_s3_bucket_object" "flag_file" {
    bucket = "${aws_s3_bucket.chal_bucket.bucket}"
    key = "youfoundme-asd897kjm.txt"
    content ="${var.flag}"
    content_type = "text/plain"
}

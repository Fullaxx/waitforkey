# waitforkey
Block until redis key exists

## Requirements
First we need to make sure we have all the appropriate libraries. \
Please consult this chart for help with installing the required packages. \
If your OS is not listed, please help us fill out the table, or submit a request via github.

| OS     | Commands (as root)                                                |
| ------ | ----------------------------------------------------------------- |
| Alpine | `apk add build-base hiredis-dev`                                  |
| CentOS | `yum install -y epel-release; yum install -y gcc hiredis-devel `  |
| Debian | `apt update; apt install -y build-essential libhiredis-dev`       |
| Fedora | `yum install -y gcc hiredis-devel`                                |
| Ubuntu | `apt update; apt install -y build-essential libhiredis-dev`       |

## Compile the code
```
./compile.sh
```

## Wait for a key
Connect to redis @ 127.0.0.1:6379 and wait for NEEDLE to appear. \
Optional: Set the delay between key checks.
```
./waitforkey.exe 127.0.0.1:6379 NEEDLE

export USLEEPDELAY=1000000
./waitforkey.exe 127.0.0.1:6379 NEEDLE
```

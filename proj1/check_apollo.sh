input=$(uname -a | awk '{print $2}')
if [ $input = "apollo09" ];then
    echo "yes"
else
    echo "no"
fi

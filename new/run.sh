if [ -d "file/" ];then
    echo "file/ exits"
else
    echo "file/ not exits"
    echo "dir file/ created "
    mkdir file
fi


exit 1
#lib req for python: requests. beautifulSoup
echo "DL documents"
python dl.py
#uncomment below to convert pptx to pdf
#echo "convert pptx to pdf"
#libreoffice --headless --invisible --convert-to pdf /Desktop/ece3220/file/*.pptx

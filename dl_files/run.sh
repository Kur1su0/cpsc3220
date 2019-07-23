if [ -d "file/" ];then
    echo "file/ exits"
else
    echo "file/ not exits"
    echo "dir file/ created "
    mkdir file
fi


#lib req for python: requests. beautifulSoup
echo "DL documents"
python dl.py
#uncomment below to convert *.pptx to *.pdf
echo "convert pptx to pdf"
if [ -d "slides/" ];then
    echo "slides/ exits"
else
    echo "slides/ not exits"
    mkdir slides
    echo "dir slides/ created "
fi
cp file/*.pptx slides/
cd slides
libreoffice --headless --invisible --convert-to pdf *.pptx

#uncomment below to remove *.pptx
rm -v *.pptx

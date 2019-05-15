from __future__ import print_function
import requests
import urllib
from bs4 import BeautifulSoup
#change to our dest-dir
path = 'file/'

image_src = []
src = []
url = 'https://people.cs.clemson.edu/~mark/3220.html'
r = requests.get(url,verify=False)
if r.status_code is 200:
    print("successfully access the website")
else:
    print("error status code",r.status_code)
    exit (1)
data = r.text

soup = BeautifulSoup(data, 'html.parser')
#print(soup.prettify())

for a in soup.find_all('a',href=True):
    if(a['href'].split('.')[-1] == 'pptx' or
       a['href'].split('.')[-1] == 'txt' or
       a['href'].split('.')[-1] == 'pdf'
    ):

        image_src.append(a['href'])
for i in range(len(image_src)):
    print(image_src[i])

for i in range(len(image_src)):
        #src.append(str(image_src[i]))
    print("downloading:",str(image_src[i]).split('/')[-1])
    with open(path + str(image_src[i]).split('/')[-1] ,'wb' ) as f:
	    f.write(  requests.get(str(image_src[i]),verify=False).content )
    print ("success...")
print("Done!")

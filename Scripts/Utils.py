import os
import sys
import time
import requests
from fake_useragent import UserAgent

def DownaloadFile(url,filepath):
    os.makedirs(os.path.dirname(filepath),exist_ok=True)
    with open(filepath, 'wb') as f:
        ua = UserAgent()
        headers = {'User-Agent':ua.chrome}
        response = requests.get(url,headers=headers,stream=True)
        total = response.headers.get('content-length')

        if total is None:
            f.write(response.content)

        else:
            downloaded = 0
            total = int(total)
            startTime = time.time()
            for data in response.iter_content(chunk_size=max(int(total/1000),1024*1024)):
                downloaded += len(data)
                f.write(data)
                done = int(50*downloaded/total)
                percentage = (downloaded/total)*100
                elapsedTime = time.time() - startTime
                avgKBps = (downloaded/1024)/elapsedTime
                avgSpeedString = '{:.2f} KB/s'.format(avgKBps)
                if(avgKBps > 1024):
                    avgMBps = avgKBps/1024
                    avgSpeedString = '{:.2f} MB/s'.format(avgMBps)
                sys.stdout.write('\r[{}{}] {:.2f}% ({})   '.format('█'*done,'.'*(50-done),percentage,avgSpeedString))
                sys.stdout.flush()
    sys.stdout.write('\n')

def YesOrNo():
    while True:
        reply = str(input('[Y/N]: ')).lower().strip()

        if reply[:1] == 'y':
            return True
        if reply[:1] == 'n':
            return False

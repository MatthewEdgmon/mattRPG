import os
import urllib.request
import zipfile

def DownloadZIP(url, extractedFolderName = None):
    zipFileName = url.rsplit('/', 1)[-1]
    assert zipFileName.endswith('.zip')
    if not extractedFolderName:
        extractedFolderName = zipFileName[:-4]
    if not os.path.exists(extractedFolderName):
        print('Downloading %s' % zipFileName)
        urllib.request.urlretrieve(url, zipFileName)
        zipfile.ZipFile(zipFileName).extractall()
        os.remove(zipFileName)
        assert os.path.exists(extractedFolderName)

thirdPartyFolder = os.path.normpath(os.path.join(__file__, '../external'))
os.makedirs(thirdPartyFolder, exist_ok=True)
os.chdir(thirdPartyFolder)

# SDL2
DownloadZIP('https://www.libsdl.org/release/SDL2-devel-2.0.16-VC.zip', 'SDL2-2.0.16')
# SDL2_image
DownloadZIP('https://www.libsdl.org/projects/SDL_image/release/SDL2_image-devel-2.0.5-VC.zip', 'SDL2_image-2.0.5')
# SDL2_mixer
DownloadZIP('https://www.libsdl.org/projects/SDL_mixer/release/SDL2_mixer-devel-2.0.4-VC.zip', 'SDL2_mixer-2.0.4')
# SDL2_net
DownloadZIP('https://www.libsdl.org/projects/SDL_net/release/SDL2_net-devel-2.0.1-VC.zip', 'SDL2_net-2.0.1')
# SDL2_ttf
DownloadZIP('https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-devel-2.0.15-VC.zip', 'SDL2_ttf-2.0.15')
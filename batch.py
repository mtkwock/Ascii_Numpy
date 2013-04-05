#Batch processing to convert edge-detected images to ascii text

from PIL import Image
from edge_ascii import ascii_to_file
import edge_ascii
from os import listdir, system, chdir, getenv, getuid, getcwd
from os.path import isfile, join
from ticktock import *
import cProfile
import charfreq
import subprocess
import sys
import re
import pwd
from main import write_to_file
from subprocess import Popen, PIPE


scm =  """; Usage: first place in .gimp-2.8/scripts
 ; Then call it with the syntax $ gimp -i -b '(edge-detect "dog.jpg" 10 255)' -b '(gimp-quit 0)'
 ; The 10 and 25 are for white value low and high thresholds.  Feel free to change these as you
 ; see fit.

 (define (edge-detect filename low high)
   (let* ((image (car (gimp-file-load RUN-NONINTERACTIVE filename filename)))
          (drawable (car (gimp-image-get-active-layer image))))
     (plug-in-laplace RUN-NONINTERACTIVE image drawable)
     (gimp-hue-saturation drawable 0 0.0 0.0 -100.0)
     (gimp-threshold drawable low high)
     ; (gimp-brightness-contrast drawable 0 contrast)
     (gimp-file-save RUN-NONINTERACTIVE image drawable "result.jpg" "result.jpg")
     (gimp-image-delete image)))

(define (edge-batch pattern low high)
(let* ((filelist (cadr (file-glob pattern 1))))
(while (not (null? filelist))
       (let* ((filename (car filelist))
              (image (car (gimp-file-load RUN-NONINTERACTIVE
                                          filename filename)))
              (drawable (car (gimp-image-get-active-layer image))))
        (plug-in-laplace RUN-NONINTERACTIVE image drawable)
        (gimp-hue-saturation drawable 0 0.0 0.0 -100.0)
        (gimp-threshold drawable low high)
         (gimp-file-save RUN-NONINTERACTIVE
                         image drawable filename filename)
         (gimp-image-delete image))
       (set! filelist (cdr filelist)))))"""

# from multiprocessing import Pool

# #where source images are stored
# input_dir = './test/'

# #where resultant images will be stored
# output_dir = './result/'

# #Used if images are processed in chunks
# last_file = ''

# #Grab a list of the files in the input directory and sort them
# #Exclude files that have already been processed (as defined by last_file)
# files = sorted([ f for f in listdir(input_dir) if isfile(join(input_dir,f)) and f > last_file])

# pool = Pool(processes=8)

# count = 0
# #open each file, process, and save in output directory
# def convert_image(i):
#     global count
#     f = files[i]
#     image = Image.open(input_dir + f)
#     name,_ = f.split('.')

#     #monitor progress
#     count += 8
#     print name+".txt" + ": file number " + str(count)

#     #the real work happens here
#     ascii_to_file(image, output_dir + name+".txt")

# if __name__ == '__main__':
#     pool.map(convert_image, range(len(files)))

<<<<<<< HEAD
# Get the duration from our input string and return a
# dictionary of hours, minutes, seconds

current_dir = getcwd()
youtube_id = ""

def get_gimp_version():
    process = Popen(['gimp', '--version'], stdout=PIPE)
    out = str(process.stdout.read())
    version = re.findall('\d+\.\d+', out)[0]
    return version

def searchForDuration (ffmpeg_output):

    pattern = re.compile(r'Duration: ([\w.-]+):([\w.-]+):([\w.-]+),')
    match = pattern.search(ffmpeg_output)

    if match:
        hours = match.group(1)
        minutes = match.group(2)
        seconds = match.group(3)
    else:
        hours = minutes = seconds = 0

    # return a dictionary containing our duration values
    return {'hours':hours, 'minutes':minutes, 'seconds':seconds}

# Get the dimensions from the specified file using ffmpeg
def getFFMPEGInfo (src_file):

    p = subprocess.Popen(['ffmpeg', '-i', src_file],stdout=subprocess.PIPE,stderr=subprocess.PIPE)
    stdout, stderr = p.communicate()
    return stderr

# Get the duration by pulling out the FFMPEG info and then
# searching for the line that contains our duration values
def getVideoDuration (src_file):

    ffmpeg_output = getFFMPEGInfo(src_file)
    return searchForDuration (ffmpeg_output)


def prep():
    global current_dir

    current_dir = str(getcwd())
    system("mkdir \'raw_images\' \'ascii_output\'")

    #Install the required dependencies
    system("sudo apt-get install ffmpeg")
    system("sudo apt-get install gimp")
    system("sudo apt-get install mencoder")

    v = get_gimp_version()
    username = pwd.getpwuid(getuid()).pw_name
    chdir("/home/" + username + "/.gimp-" + v +"/scripts/")
    f = open("edge-detect.scm", 'w+')
    f.write(scm)
    f.close()

def clean_up():
    system("rm -r \'raw_images\' \'ascii_input\' \'ascii_output\'")

def youtube_dl():
    global youtube_id
    chdir(current_dir)
    youtube_url = str(raw_input("Enter the youtube url of the movie you want to convert: "))
    _, youtube_id = youtube_url.split("v=")
    chdir('raw_images')
    system("youtube-dl -f 22 " + youtube_id)
    time = getVideoDuration(youtube_id + ".mp4")
    system("ffmpeg -ss 00:00:00 -t " + time['hours'] +  ":" + time['minutes'] \
            + ":" + time['seconds'] + " -i " + youtube_id + ".mp4 -r 30.0 %8d.jpg")

    #might be lossy -- could potentially rip as aac
    system("ffmpeg -i " + youtube_id + ".mp4 -ab 128k -ac 2 -ar 44100 -vn sound.mp4")

def edge_detect():
    #the value should be variable but is set to ten for now.
    chdir(current_dir + "/raw_images")
    system("gimp -i -b '(edge-batch \"*.jpg\" 7 255)' -b '(gimp-quit 0)'")


def begin():
    #reset time
    tick()

    chdir(current_dir)

    #where source images are stored
    input_dir = './raw_images/'

    #where resultant images will be stored
    output_dir = './ascii_output/'

    #Used if images are processed in chunks
    last_file = ''

    #Grab a list of the files in the input directory and sort them
    #Exclude files that have already been processed (as defined by last_file)
    edge_ascii.set_occurence(charfreq.get_most_frequent(input_dir))
    files = sorted([ f for f in listdir(input_dir) if isfile(join(input_dir,f)) and f > last_file])
    for f in files:
        image = Image.open(input_dir + f)
        print f
        name,_ = f.split('.')

        #the real work happens here
        ascii_to_file(image, output_dir + name+".txt")
        system("sudo rm " + input_dir + f)
        print name+".txt"

        #output time
        tock()

def finalize():
    chdir(current_dir)
    system("python txt_to_pic.py")
    system("ffmpeg -i " + "./raw_images/" + "abjAqvdGZgM" + ".mp4 -ab 128k -ac 2 -ar 44100 -strict experimental -vn ascii_output/sound.mp4")
    system("sudo mv ./raw_images/sound.mp4 ./ascii_output")
    chdir("ascii_output")
    system("mencoder mf://*.jpg -mf w=1280:h=720:fps=30:type=jpg -ovc lavc \
           -lavcopts vcodec=mpeg4:mbd=2:trell -oac copy -o video.avi")
    chdir(current_dir)
    system("ffmpeg -i sound.mp4 -i video.avi -acodec copy -vcodec copy ascii.mp4")
    # system("mv ascii.mp4 ../")
    chdir(current_dir)

def begin_smart():
    #reset time
    tick()

    #where source images are stored
    input_dir = './in/'

    #where resultant images will be stored
    output_dir = './out/'

    #Used if images are processed in chunks
    last_file = ''

    file_counter = 0
    #Grab a list of the files in the input directory and sort them
    #Exclude files that have already been processed (as defined by last_file)
    edge_ascii.set_occurence(charfreq.get_most_frequent(input_dir, 1))
    files = sorted([ f for f in listdir(input_dir) if isfile(join(input_dir,f)) and f > last_file])
    for f in files:
        image = Image.open(input_dir + f)
#       print f
        name,_ = f.split('.')

        #the real work happens here
        ascii_to_file(image, output_dir + name+".txt")

        file_counter = file_counter + 1
        #output time
        t = tock(True)
        print(name+".txt was made")
        print("Time Elapsed: " + str(t)[:7] + "   Average time: " + str(float(t)/file_counter)[:7])

def begin_gradient():
    #reset time
    tick()

    #where source images are stored
    input_dir = './in/'

    #where resultant images will be stored
    output_dir = './out2/'

    #Used if images are processed in chunks
    last_file = ''

    file_counter = 0
    #Grab a list of the files in the input directory and sort them
    #Exclude files that have already been processed (as defined by last_file)
#   edge_ascii.set_occurence(charfreq.get_most_frequent(input_dir))
    files = sorted([ f for f in listdir(input_dir) if isfile(join(input_dir,f)) and f > last_file])
    for f in files:
        if(f[f.index('.'):len(f)] is not ".txt"):
            continue
        image = input_dir + f
#       print f
        name,_ = f.split('.')

        #the real work happens here
        write_to_file(image, output_dir + name+".txt")

        file_counter = file_counter + 1
        #output time
        t = tock(True)
        print(name+".txt was made")
        print("Time Elapsed: " + str(t)[:7] + "   Average time: " + str(float(t)/file_counter)[:7])

if(__name__ == "__main__"):
    prep()
    youtube_dl()
    edge_detect()
    begin()
    finalize()
    clean_up()


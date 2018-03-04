
# PwnAdventure3
![PwnAdventure3](http://www.pwnadventure.com/img/logo.png)

Welcome to Pwnie Island!

> Pwn Adventure 3: Pwnie Island is a limited-release, first-person, true open-world MMORPG set on a beautiful island where anything could happen. That's because this game is intentionally vulnerable to all kinds of silly hacks! Flying, endless cash, and more are all one client change or network proxy away. Are you ready for the mayhem?!

- Official Site: [http://www.pwnadventure.com/](http://www.pwnadventure.com/)

## Install Server

### Requirements

From the official README

* At least 2GB of RAM, more RAM will allow more instances to be run on a single machine
* The Game Server does not need any graphics hardware and runs purely on console. It is known to run well on Amazon AWS and Digital Ocean VPS instances.
* The Game Server requires a lot of RAM to run, but uses fork and copy-on-write memory to allow many instances to run on a single host.  
* For a server with 2GB of RAM, it is not recommended to run more than 5 instances, but a server with 8GB of RAM can typically run as many as the CPU can handle. 
* It is recommended to use 2-3 instances per CPU core if you have sufficient RAM.  You may be able to run 4-5 instances per core, but doing so may introduce slight lag.

* The `pwn3.tar.gz` file containing the client and server files is 1.8G. So several GB of free space is required as well.

There are several ways to build and deploy your own server.

### Option 1 - Original
One option is to download and follow the instructions included in the official files, or use the docker compose files provided by this repository. The original files on the official website can be found here [http://www.pwnadventure.com/#server](http://www.pwnadventure.com/#server).

### Option 2 - Guide
[@Beaujeant](https://twitter.com/Beaujeant) created an excellent and easy to follow step-by-step guide, using the official files from Option 1. It was also the basis for building the docker image from Option 3. It can be found here: [https://github.com/beaujeant/PwnAdventure3/blob/master/INSTALL-server.md](https://github.com/beaujeant/PwnAdventure3/blob/master/INSTALL-server.md). 

### Option 3 - Docker
The server requires large files from the client. Unfortunately the files for the client are downloaded via the Launcher requiring a GUI. Because of that, and in case the hosted files disappear, the files are mirrored on Amazon S3. The `pwn3.tar.gz` contains the Linux `client/` and `server/` files.

```bash
git clone https://github.com/LiveOverflow/PwnAdventure3.git
cd PwnAdventure3
wget https://s3.eu-central-1.amazonaws.com/pwnadventure3/pwn3.tar.gz
tar -xvf pwn3.tar.gz
```

In order to run the server `docker` and `docker-compose` have to be installed. Docker is moving fast, so it's a good idea to remove the system docker installation and follow the current official steps for installation: 

* Docker CE Ubuntu: [https://docs.docker.com/install/linux/docker-ce/ubuntu/](https://docs.docker.com/install/linux/docker-ce/ubuntu/). 
* `docker-compose`: [https://docs.docker.com/compose/install/](https://docs.docker.com/compose/install/)
* make sure the current user is part of the `docker` group with: `sudo usermod -a -G docker $USER`. restart or relogin and verify that the user has the group with `id`.

```bash
docker-compose build
docker-compose up
```

`docker-compose up` can also run in detached/background mode with `-d`.

To get a client connected to the server, the `server.ini` for the client  has to be modified. The server expects that hostnames `master.pwn3` and `game.pwn3` are being used. Change client `server.ini` like this:

```
[MasterServer]
Hostname=master.pwn3
Port=3333

[GameServer]
Hostname=game.pwn3
Port=3000
Username=
Password=
Instances=
```
Make sure that the client can reache these hosts, for example by adding them to the `/etc/hosts`. In this example the server is running on `192.168.178.57`:

```
192.168.178.57  master.pwn3
192.168.178.57  game.pwn3
```

**Warning:** Using an IP as `Hostname` in the `server.ini` does not work! I spent 2 hours trying to figure out what was wrong.


To stop the server, simply type `docker-compose down`

## Troubleshooting

### Error: docker-compose build
```bash
$ docker-compose build
Building init
ERROR: Error processing tar file(exit status 1): write /client/PwnAdventure3_Data/PwnAdventure3/PwnAdventure3/Content/Paks/Characters.pak: no space left on device
```
A: Get more disk space.

```bash
$ docker-compose build
Building init
ERROR: Couldn't connect to Docker daemon at http+docker://localunixsocket - is it running?
```

A: Your user is probably not part of the `docker` group or docker service not running. `sudo usermod -a -G docker pwn3`, verify with `id`. Or `service docker restart`.

### File Integrity

Check if the archive was corrupted

```bash
$ md5sum pwn3.tar.gz
d3f296461fa57996018ce0e4e5a653ee  pwn3.tar.gz
$ sha1sum pwn3.tar.gz
022bd5174286fd78cd113bc6da6d37ae9af1ae8e  pwn3.tar.gz
```

### PwnAdventure3 Client Errors

**Connection Error: Unable to connect to master server**
MasterServer is not reachable.

* Client issues:
  * Check the `[MasterServer]` entry in the client's `server.ini`
  * Can you ping `master.pwn3` from the host from your system?
  * Is the IP correct in the `/etc/hosts` file?
* Server issues:
  * Is the server not running and listening on port `3333`? 
  * Check with `sudo netstat -tulpn`
    * Is the master server listening: `tcp6 0 0 :::3333 :::* LISTEN 31913/docker-proxy`
  * Check `docker ps` if the two containers are up
    * master server running? `880f93374070 pwn3server "/opt/pwn3/setup/mas…" 11 seconds ago Up 10 seconds 0.0.0.0:3333->3333/tcp, 5432/tcp pwnadventure3_master_1`

**Waiting in connection queue...**
MasterServer reachable and provided the client a GameServer to connect to, but it's not running.
* Server issues:
  * Is the game server running and listening on port `3000`? 
  *  Check with `sudo netstat -tulpn`
    * `tcp6 0 0 :::3000 :::* LISTEN 32160/docker-proxy`
  * Is `pwnadventure3_game_1` container Status: running? Check with `docker ps -a`
    * `84343f81034f pwn3server "/opt/pwn3/setup/gam…" 8 seconds ago Up 7 seconds        0.0.0.0:3000-3010->3000-3010/tcp, 5432/tcp pwnadventure3_game_1`
  * do you see `line 1: 7 Killed ./PwnAdventure3Server; pwnadventure3_game_1 exited with code 137` in the log?
      * GET MORE RAM!

### Docker versions

These versions were used during testing:

```bash
$ uname -a
Linux ubuntu 4.4.0-31-generic #50~14.04.1-Ubuntu SMP Wed Jul 13 01:07:32 UTC 2016 x86_64 x86_64 x86_64 GNU/Linux
$ docker-compose version
docker-compose version 1.19.0, build 9e633ef
docker-py version: 2.7.0
CPython version: 2.7.13
OpenSSL version: OpenSSL 1.0.1t  3 May 2016
$ docker --version
Docker version 17.12.1-ce, build 7390fc6
```


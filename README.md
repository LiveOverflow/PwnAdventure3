
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

There are several ways to build and deploy your own server.

### Option 1 - Original
One option is to download and follow the instructions included in the official files, or use the docker compose files provided by this repository. The original files on the official website can be found here [http://www.pwnadventure.com/#server](http://www.pwnadventure.com/#server).

### Option 2 - Guide
An  easy to follow step-by-step guide using the official files from Option 1 by [@Beaujeant](https://twitter.com/Beaujeant) can be found here [https://pacmans.army/?p=143](https://web.archive.org/web/20160201054425/https://pacmans.army/?p=143). 

### Option 3 - Docker
The server requires large files from the client. Unfortunately the files for the client are downloaded via the Launcher requiring a GUI. Because of that, and in case the hosted files disappear, the files are mirrored on Amazon S3. The `pwn3.tar.gz` contains the Linux `client/` and `server/` files.

```bash
git clone https://github.com/LiveOverflow/PwnAdventure3.git
cd PwnAdventure3
wget https://s3.eu-central-1.amazonaws.com/pwnadventure3/pwn3.tar.gz
tar -xvf pwn3.tar.gz
```

In order to run the server `docker` and `docker-compose` have to be installed. Docker is moving fast, so it's a good idea to remove the system docker installation and follow the current official steps for installation. 

* Docker CE Ubuntu: [https://docs.docker.com/install/linux/docker-ce/ubuntu/](https://docs.docker.com/install/linux/docker-ce/ubuntu/). 
* `docker-compose`: [https://docs.docker.com/compose/install/](https://docs.docker.com/compose/install/)

```bash
docker-compose build
docker-compose up
```

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

## Troubleshooting

### Error: docker-compose build
```bash
$ docker-compose build
Building init
ERROR: Error processing tar file(exit status 1): write /client/PwnAdventure3_Data/PwnAdventure3/PwnAdventure3/Content/Paks/Characters.pak: no space left on device
```
A: Get more disk space.

### File Integrity

Check if the archive was corrupted

```bash
$ md5sum pwn3.tar.gz
d3f296461fa57996018ce0e4e5a653ee  pwn3.tar.gz
$ sha1sum pwn3.tar.gz
022bd5174286fd78cd113bc6da6d37ae9af1ae8e  pwn3.tar.gz
```

### Docker versions

These versions were used during testing:

```bash
$ docker-compose version
docker-compose version 1.19.0, build 9e633ef
docker-py version: 2.7.0
CPython version: 2.7.13
OpenSSL version: OpenSSL 1.0.1t  3 May 2016
$ docker --version
Docker version 17.12.1-ce, build 7390fc6
```


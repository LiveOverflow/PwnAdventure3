#!/bin/sh
service postgresql start
su pwn3 -c "rm /opt/pwn3/client/PwnAdventure3_Data/PwnAdventure3/PwnAdventure3/Saved/Logs/*"
su pwn3 -c "cd /opt/pwn3/server/MasterServer/ && ./MasterServer --create-server-account > /opt/pwn3/server/creds"
USER=$(cat /opt/pwn3/server/creds | grep 'Username:' | cut -d ":" -f 2- | xargs)
PW=$(cat /opt/pwn3/server/creds | grep 'Password:' | cut -d ":" -f 2- | xargs)
cat >/opt/pwn3/client/PwnAdventure3_Data/PwnAdventure3/PwnAdventure3/Content/Server/server.ini <<EOL
[MasterServer]
Hostname=master.pwn3
Port=3333

[GameServer]
Hostname=game.pwn3
Port=3000
Username=$USER
Password=$PW
Instances=5
EOL
su pwn3 -c "cd /opt/pwn3/server/MasterServer/ && ./MasterServer --create-admin-team Admin"
su pwn3 -c "cd /opt/pwn3/server/MasterServer/ && ./MasterServer"

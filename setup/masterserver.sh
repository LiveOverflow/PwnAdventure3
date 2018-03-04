#!/bin/sh
service postgresql start

su postgres -c "psql -f $PWN3/setup/postgres_init.sql -d template1"
su pwn3 -c "psql master -f $PWN3/initdb.sql"
su pwn3 -c "psql master -f $PWN3/setup/postgres_master.sql"

# clean up
su pwn3 -c "rm /opt/pwn3/client/PwnAdventure3_Data/PwnAdventure3/PwnAdventure3/Saved/Logs/*"

# get the master server creads
su pwn3 -c "cd /opt/pwn3/server/MasterServer/ && ./MasterServer --create-server-account > /opt/pwn3/server/creds"

# write the creds to the server.ini
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
# run the server
su pwn3 -c "cd /opt/pwn3/server/MasterServer/ && ./MasterServer"

#!/bin/bash 

_term() {
  if [ -z "$child" ]
  then
	return 0
  fi
  echo "Caught SIGTERM signal!" 
  trap - SIGTERM SIGKILL # clear the trap   
  # kill sounds mean, but -s is graceful  
  kill -s -TERM "$child" 2>/dev/null
  su postgres -c "pg_dump --schema=public --format=plain --file=/opt/pwn3/postgres-data/data.sql master"
  exit 0
}

trap _term SIGTERM SIGKILL

service postgresql start

sleep 10

su postgres -c "psql -f $PWN3/setup/postgres_init.sql -d template1"

# clean up
su pwn3 -c "rm /opt/pwn3/client/PwnAdventure3_Data/PwnAdventure3/PwnAdventure3/Saved/Logs/*"

if [ -f /opt/pwn3/postgres-data/data.sql ]; then
    echo "Found data, making a backup now!"
	cp -p /opt/pwn3/postgres-data/data.sql /opt/pwn3/postgres-data/data.sql."$(date +%Y%m%d_%H%M%S)"
	echo "Restoring backup data!"
	su postgres -c "psql -U postgres --dbname master -1 --file /opt/pwn3/postgres-data/data.sql"
else
    echo "No backup data found!"
	su pwn3 -c "psql master -f $PWN3/initdb.sql"
	su pwn3 -c "psql master -f $PWN3/setup/postgres_master.sql"
	su pwn3 -c "cd /opt/pwn3/server/MasterServer/ && ./MasterServer --create-admin-team Admin"
fi

# cleanup all previous/old server master creds
su pwn3 -c "psql master -f $PWN3/setup/postgres_cleanup.sql"

# get new master server creds
su pwn3 -c "cd /opt/pwn3/server/MasterServer/ && ./MasterServer --create-server-account > /opt/pwn3/server/creds"

# write the new creds to the server.ini
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


# run the server
su pwn3 -c "cd /opt/pwn3/server/MasterServer/ && ./MasterServer"  &


child=$! 
wait "$child"
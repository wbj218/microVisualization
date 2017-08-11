import subprocess as sp

MONGO_USER_ACCOUNT_PORT = 32030
MMC_USER_ACCOUNT_PORT = 32031

sp.call("docker stop mongo-user_account", shell = True)
sp.call("docker stop mmc-user_account", shell = True)

sp.call("docker rm mongo-user_account", shell = True)
sp.call("docker rm mmc-user_account", shell = True)

sp.call ("docker volume rm $(docker volume ls -f dangling=true -q)", shell = True)

sp.call ("docker run -d -p " + str(MONGO_USER_ACCOUNT_PORT) + ":27017 --name mongo-user_account" + " mongo", shell = True)
sp.call ("docker run -d -p " + str(MMC_USER_ACCOUNT_PORT) + ":11211 --name mmc-user_account" + " memcached", shell = True)
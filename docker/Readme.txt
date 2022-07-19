How to start:
To start the docker container use the command "docker-compose up" inside the docker folder
If you need to rebuild the images use "docker-compose build"

Now to setup the server make sure you have the correct configuration and the data folder in the correct location:
The configuration should be place here "../../etc" from the docker folder
The data folder should be place here "../../data" from the docker folder
Note: those paths can be change inside the docker-compose.yml file

Configutation:
Inside your "worldserver.conf" make sure the "DataDir" is set to "/firelands/data"
Make sure all the mysql hostname is set to "mysqldb"
The Mysql connection seting is set inside the docker-compose.yml file and can be change
Here the default values:
- MYSQL_USER=firelands
- MYSQL_PASSWORD=firelands
- MYSQL_ROOT_PASSWORD=firelands
- MYSQL_ROOT_HOST=%

The database need to be run manualy in order to have it initialize
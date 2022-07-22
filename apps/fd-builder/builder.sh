current_date=$(date +"%Y_%m_%d_%H%M")
echo "Start building Firelands Database"
cd ../../data
mkdir build
cat ./sql/base/**/*.sql > build/FDB_$current_date.sql
echo "Build process Finished"

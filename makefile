all:
	g++ sql.cpp    `mysql_config --libs`

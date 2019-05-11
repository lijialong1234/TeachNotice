%1 mshta vbscript:CreateObject("Shell.Application").ShellExecute("cmd.exe","/c %~s0 ::","","runas",1)(window.close)&&exit
@echo off
echo "================================MIAN YANG MEIKE====================================="
del /F my.ini
echo [client] >> my.ini
echo port = 3306 >> my.ini
echo password = 123 >> my.ini
echo socket  = /tmp/mysql.sock >> my.ini
echo default_character_set=utf8 >> my.ini
echo [mysqld] >> my.ini
echo port = 3306 >> my.ini
echo basedir=%MYSQL_HOME% >>my.ini
echo datadir=%MYSQL_HOME%data >> my.ini
echo general_log=1 >> my.ini
echo #slow_query_log=1 >> my.ini
echo #log=1 >> my.ini
echo #log_output = %MYSQL_HOME%mylog.log >> my.ini
echo log_error = %MYSQL_HOME%err.log >> my.ini
echo default_storage_engine=InnoDB >> my.ini
echo sql-mode="STRICT_TRANS_TABLES,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION"  >> my.ini
echo character_set_server=utf8 >> my.ini
echo max_connections=500  >> my.ini
echo query_cache_size=128M  >> my.ini
echo query_cache_limit=1M  >> my.ini
echo tmp_table_size=34M >> my.ini
echo thread_cache_size=8  >> my.ini
echo myisam_max_sort_file_size=100G >> my.ini
echo myisam_sort_buffer_size=68M >> my.ini
echo key_buffer_size=54M  >> my.ini
echo read_buffer_size=4M  >> my.ini
echo read_rnd_buffer_size=4M >> my.ini
echo sort_buffer_size=4M >> my.ini
echo innodb_flush_log_at_trx_commit=1 >> my.ini
echo innodb_log_buffer_size=2M >> my.ini
echo innodb_buffer_pool_size=105M >> my.ini
echo innodb_log_file_size=53M >> my.ini
echo innodb_thread_concurrency=10 >> my.ini
echo "my.ini in %MYSQL_HOME% created."

set nowpath=%~dp0%
%~d0
cd "%nowpath%\bin"
wmic ENVIRONMENT where "name='path' and username='<system>'" set VariableValue="%path%;%nowpath%bin"
net stop StochasticServer
mysqld --remove StochasticServer
mysqld install StochasticServer --defaults-file="%nowpath%\my.ini"
net start StochasticServer
pause

echo .........................................................................................
echo Finished creating service "StochasticServer". Please check it in the service panel.
echo .........................................................................................
set nowpath=%~dp0%
%~d0
cd "%nowpath%\bin"
net stop TeachingNotice
mysqld --remove TeachingNotice
mysqld install TeachingNotice --defaults-file="%nowpath%\my.ini"
net start TeachingNotice
pause
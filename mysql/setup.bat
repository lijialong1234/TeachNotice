set nowpath=%~dp0%
%~d0
cd "%nowpath%\bin"
wmic ENVIRONMENT where "name='path' and username='<system>'" set VariableValue="%path%;%nowpath%bin"
net stop TeachingNotice
mysqld --remove TeachingNotice
mysqld install TeachingNotice --defaults-file="%nowpath%\my.ini"
net start TeachingNotice
pause

echo .........................................................................................
echo Finished creating service "TeachingNotice". Please check it in the service panel.
echo .........................................................................................
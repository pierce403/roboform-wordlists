$number = 8  # number of characters in the password

# List of commands to run sequentially
$commands = @(
# uld
    "& .\RoboFormWrapper.exe 1325376000 1341187200 7 $number 1 | Out-File -FilePath .\2012-${number}uld.a.txt -Encoding utf8",
    "& .\RoboFormWrapper.exe 1341187201 1356998400 7 $number 1 | Out-File -FilePath .\2012-${number}uld.b.txt -Encoding utf8",
    "& .\RoboFormWrapper.exe 1356998400 1372809600 7 $number 1 | Out-File -FilePath .\2013-${number}uld.a.txt -Encoding utf8",
    "& .\RoboFormWrapper.exe 1372809601 1388534400 7 $number 1 | Out-File -FilePath .\2013-${number}uld.b.txt -Encoding utf8",
    "& .\RoboFormWrapper.exe 1388534400 1404345600 7 $number 1 | Out-File -FilePath .\2014-${number}uld.a.txt -Encoding utf8",
    "& .\RoboFormWrapper.exe 1404345601 1420070400 7 $number 1 | Out-File -FilePath .\2014-${number}uld.b.txt -Encoding utf8",
    "& .\RoboFormWrapper.exe 1420070400 1435881600 7 $number 1 | Out-File -FilePath .\2015-${number}uld.a.txt -Encoding utf8",
    "& .\RoboFormWrapper.exe 1435881601 1451606400 7 $number 1 | Out-File -FilePath .\2015-${number}uld.b.txt -Encoding utf8",
    "& .\RoboFormWrapper.exe 1451606400 1467417600 7 $number 1 | Out-File -FilePath .\2016-${number}uld.a.txt -Encoding utf8",
    "& .\RoboFormWrapper.exe 1467417601 1483228800 7 $number 1 | Out-File -FilePath .\2016-${number}uld.b.txt -Encoding utf8",
# ulds
    "& .\RoboFormWrapper.exe 1325376000 1341187200 15 $number 1 | Out-File -FilePath .\2012-${number}ulds.a.txt -Encoding utf8",
    "& .\RoboFormWrapper.exe 1341187201 1356998400 15 $number 1 | Out-File -FilePath .\2012-${number}ulds.b.txt -Encoding utf8",
    "& .\RoboFormWrapper.exe 1356998400 1372809600 15 $number 1 | Out-File -FilePath .\2013-${number}ulds.a.txt -Encoding utf8",
    "& .\RoboFormWrapper.exe 1372809601 1388534400 15 $number 1 | Out-File -FilePath .\2013-${number}ulds.b.txt -Encoding utf8",
    "& .\RoboFormWrapper.exe 1388534400 1404345600 15 $number 1 | Out-File -FilePath .\2014-${number}ulds.a.txt -Encoding utf8",
    "& .\RoboFormWrapper.exe 1404345601 1420070400 15 $number 1 | Out-File -FilePath .\2014-${number}ulds.b.txt -Encoding utf8",
    "& .\RoboFormWrapper.exe 1420070400 1435881600 15 $number 1 | Out-File -FilePath .\2015-${number}ulds.a.txt -Encoding utf8",
    "& .\RoboFormWrapper.exe 1435881601 1451606400 15 $number 1 | Out-File -FilePath .\2015-${number}ulds.b.txt -Encoding utf8",
    "& .\RoboFormWrapper.exe 1451606400 1467417600 15 $number 1 | Out-File -FilePath .\2016-${number}ulds.a.txt -Encoding utf8",
    "& .\RoboFormWrapper.exe 1467417601 1483228800 15 $number 1 | Out-File -FilePath .\2016-${number}ulds.b.txt -Encoding utf8"
)

# Running the commands sequentially
foreach ($cmd in $commands) {
    Write-Output "Running: $cmd"
    Invoke-Expression $cmd
    if ($LASTEXITCODE -ne 0) {
        Write-Output "Command failed with exit code ${LASTEXITCODE}: $cmd"
        break
    }
}

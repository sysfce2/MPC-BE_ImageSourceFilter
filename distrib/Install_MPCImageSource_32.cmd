@cd /d "%~dp0"
@regsvr32.exe MpcImageSource.ax /s
@if %errorlevel% NEQ 0 goto error
:success
@echo.
@echo.
@echo    Installation succeeded.
@echo.
@echo    Please do not delete the MpcVImageSource.ax file.
@echo    The installer has not copied the files anywhere.
@echo.
@goto done
:error
@echo.
@echo.
@echo    Installation failed.
@echo.
@echo    You need to right click "Install_MPCImageSource_32.cmd" and choose "run as admin".
@echo.
:done
@pause >NUL

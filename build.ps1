Param(
    [Parameter(Mandatory = $False)]
    [Switch]$Upload
)
Move-Item .\examples\BasicOTA\BasicOTA.ino .\examples\BasicOTA\BasicOTA.cpp

try {
    if ($Upload) {
        pio run -t "upload"
    }
    else {
        pio run
    }
}
finally {
    Move-Item .\examples\BasicOTA\BasicOTA.cpp .\examples\BasicOTA\BasicOTA.ino
}
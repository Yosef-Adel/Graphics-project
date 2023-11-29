param([string[]] $tests)

function Invoke-Tests {
    param([string[]] $configs)
    foreach ($config in $configs){
        ./bin/GAME_APPLICATION -f=2 -c="$config"
    }
}


###################################################
###################################################

if( ($tests.Count -eq 0) -or ($tests -contains "renderer-test")){
    $configs = @(
        "config/renderer-test/test-0.jsonc",
        "config/renderer-test/test-1.jsonc"
    )
    Write-Output ""
    Write-Output "Running renderer-test:"
    Write-Output ""
    Invoke-Tests $configs
}

###################################################
###################################################

if( ($tests.Count -eq 0) -or ($tests -contains "sky-test")){
    $configs = @(
        "config/sky-test/test-0.jsonc",
        "config/sky-test/test-1.jsonc"
    )
    Write-Output ""
    Write-Output "Running sky-test:"
    Write-Output ""
    Invoke-Tests $configs
}

###################################################
###################################################

if( ($tests.Count -eq 0) -or ($tests -contains "postprocess-test")){
    $configs = @(
        "config/postprocess-test/test-0.jsonc",
        "config/postprocess-test/test-1.jsonc",
        "config/postprocess-test/test-2.jsonc",
        "config/postprocess-test/test-3.jsonc"
    )
    Write-Output ""
    Write-Output "Running postprocess-test:"
    Write-Output ""
    Invoke-Tests $configs
}
param([string[]] $tests)

function Invoke-Tests {
    param([string[]] $configs)
    foreach ($config in $configs){
        ./bin/GAME_APPLICATION -f=2 -c="$config"
    }
}


###################################################
###################################################

if( ($tests.Count -eq 0) -or ($tests -contains "texture-test")){
    $configs = @(
        "config/texture-test/test-0.jsonc"
    )
    Write-Output ""
    Write-Output "Running texture-test:"
    Write-Output ""
    Invoke-Tests $configs
}

###################################################
###################################################

if( ($tests.Count -eq 0) -or ($tests -contains "sampler-test")){
    $configs = @(
        "config/sampler-test/test-0.jsonc",
        "config/sampler-test/test-1.jsonc",
        "config/sampler-test/test-2.jsonc",
        "config/sampler-test/test-3.jsonc",
        "config/sampler-test/test-4.jsonc",
        "config/sampler-test/test-5.jsonc",
        "config/sampler-test/test-6.jsonc",
        "config/sampler-test/test-7.jsonc"
    )
    Write-Output ""
    Write-Output "Running sampler-test:"
    Write-Output ""
    Invoke-Tests $configs
}

###################################################
###################################################

if( ($tests.Count -eq 0) -or ($tests -contains "material-test")){
    $configs = @(
        "config/material-test/test-0.jsonc",
        "config/material-test/test-1.jsonc"
    )
    Write-Output ""
    Write-Output "Running material-test:"
    Write-Output ""
    Invoke-Tests $configs
}

###################################################
###################################################

if( ($tests.Count -eq 0) -or ($tests -contains "entity-test")){
    $configs = @(
        "config/entity-test/test-0.jsonc",
        "config/entity-test/test-1.jsonc"
    )
    Write-Output ""
    Write-Output "Running entity-test:"
    Write-Output ""
    Invoke-Tests $configs
}

param([string[]] $tests)

function Invoke-Tests {
    param([string[]] $configs)
    foreach ($config in $configs){
        ./bin/GAME_APPLICATION -f=2 -c="$config"
    }
}

###################################################
###################################################

if( ($tests.Count -eq 0) -or ($tests -contains "shader-test")){
    $configs = @(
        "config/shader-test/test-0.jsonc",
        "config/shader-test/test-1.jsonc",
        "config/shader-test/test-2.jsonc",
        "config/shader-test/test-3.jsonc",
        "config/shader-test/test-4.jsonc",
        "config/shader-test/test-5.jsonc",
        "config/shader-test/test-6.jsonc",
        "config/shader-test/test-7.jsonc",
        "config/shader-test/test-8.jsonc",
        "config/shader-test/test-9.jsonc"
    )
    Write-Output ""
    Write-Output "Running shader-test:"
    Write-Output ""
    Invoke-Tests $configs
}

###################################################
###################################################

if( ($tests.Count -eq 0) -or ($tests -contains "mesh-test")){
    $configs = @(
        # Default Mesh
        "config/mesh-test/default-0.jsonc",
        "config/mesh-test/default-1.jsonc",
        "config/mesh-test/default-2.jsonc",
        "config/mesh-test/default-3.jsonc",

        # Monkey Mesh
        "config/mesh-test/monkey-0.jsonc",
        "config/mesh-test/monkey-1.jsonc",
        "config/mesh-test/monkey-2.jsonc",
        "config/mesh-test/monkey-3.jsonc"
    )
    Write-Output ""
    Write-Output "Running mesh-test:"
    Write-Output ""
    Invoke-Tests $configs
}

###################################################
###################################################

if( ($tests.Count -eq 0) -or ($tests -contains "transform-test")){
    $configs = @(
        "config/transform-test/test-0.jsonc"
    )
    Write-Output ""
    Write-Output "Running transform-test:"
    Write-Output ""
    Invoke-Tests $configs
}

###################################################
###################################################

if( ($tests.Count -eq 0) -or ($tests -contains "pipeline-test")){
    $configs = @(
        # Face Culling
        "config/pipeline-test/fc-0.jsonc",
        "config/pipeline-test/fc-1.jsonc",
        "config/pipeline-test/fc-2.jsonc",
        "config/pipeline-test/fc-3.jsonc",

        # Depth Testing
        "config/pipeline-test/dt-0.jsonc",
        "config/pipeline-test/dt-1.jsonc",
        "config/pipeline-test/dt-2.jsonc",

        # Blending
        "config/pipeline-test/b-0.jsonc",
        "config/pipeline-test/b-1.jsonc",
        "config/pipeline-test/b-2.jsonc",
        "config/pipeline-test/b-3.jsonc",
        "config/pipeline-test/b-4.jsonc",

        # Mask
        "config/pipeline-test/cm-0.jsonc",
        "config/pipeline-test/dm-0.jsonc"
    )
    Write-Output ""
    Write-Output "Running pipeline-test:"
    Write-Output ""
    Invoke-Tests $configs
}

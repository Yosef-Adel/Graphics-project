param([string[]] $tests)

$failure = 0




###################################################
###################################################

$requirement = "texture-test"
if( ($tests.Count -eq 0) -or ($tests -contains $requirement)){
    $files = @(
        "test-0.png"
    )
    Write-Output ""
    Write-Output "Comparing $requirement output:"
    & "./scripts/compare-group.ps1" -requirement $requirement -files $files -tolerance 0.01 -threshold 0
    $failure += $LASTEXITCODE
}

###################################################
###################################################

$requirement = "sampler-test"
if( ($tests.Count -eq 0) -or ($tests -contains $requirement)){
    $files = @(
        "test-0.png",
        "test-1.png",
        "test-2.png",
        "test-3.png",
        "test-4.png",
        "test-5.png",
        "test-6.png",
        "test-7.png"
    )
    Write-Output ""
    Write-Output "Comparing $requirement output:"
    & "./scripts/compare-group.ps1" -requirement $requirement -files $files -tolerance 0.01 -threshold 0
    $failure += $LASTEXITCODE
}

###################################################
###################################################

$requirement = "material-test"
if( ($tests.Count -eq 0) -or ($tests -contains $requirement)){
    $files = @(
        "test-0.png",
        "test-1.png"
    )
    Write-Output ""
    Write-Output "Comparing $requirement output:"
    & "./scripts/compare-group.ps1" -requirement $requirement -files $files -tolerance 0.02 -threshold 64
    $failure += $LASTEXITCODE
}

###################################################
###################################################

$requirement = "entity-test"
if( ($tests.Count -eq 0) -or ($tests -contains $requirement)){
    $files = @(
        "test-0.png",
        "test-1.png"
    )
    Write-Output ""
    Write-Output "Comparing $requirement output:"
    & "./scripts/compare-group.ps1" -requirement $requirement -files $files -tolerance 0.04 -threshold 64
    $failure += $LASTEXITCODE
}

###################################################
###################################################
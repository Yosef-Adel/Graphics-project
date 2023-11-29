param([string[]] $tests)

$failure = 0

###################################################
###################################################

$requirement = "renderer-test"
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

$requirement = "sky-test"
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

$requirement = "postprocess-test"
if( ($tests.Count -eq 0) -or ($tests -contains $requirement)){
    $files = @(
        "test-0.png",
        "test-1.png",
        "test-2.png",
        "test-3.png"
    )
    Write-Output ""
    Write-Output "Comparing $requirement output:"
    & "./scripts/compare-group.ps1" -requirement $requirement -files $files -tolerance 0.04 -threshold 64
    $failure += $LASTEXITCODE
}

############################
############################
############################

Write-Output ""
Write-Output "Overall Results"
if($failure -eq 0){
    Write-Output "SUCCESS: All outputs are correct"
}else{
    if($failure -eq 1){
        Write-Output "FAILURE: $failure output is incorrect"
    }else{
        Write-Output "FAILURE: $failure outputs are incorrect"
    }
}
# PowerShell build script for srms_gui
Write-Host "Attempting to build with g++ (MinGW)..."
try {
    & g++ srms_gui.cpp -o srms_gui.exe -static -lgdi32 -lmsimg32
    if ($LASTEXITCODE -eq 0) {
        Write-Host "Built srms_gui.exe with g++."
        exit 0
    }
} catch {
    # ignore
}

Write-Host "g++ build failed or not found. Attempting Visual Studio cl.exe..."
try {
    & cl /EHsc srms_gui.cpp user32.lib gdi32.lib
    if ($LASTEXITCODE -eq 0) {
        Write-Host "Built srms_gui.exe with cl.exe."
        exit 0
    }
} catch {
    # ignore
}

Write-Host "Build failed. Ensure MinGW (g++) or Visual Studio (cl) is installed and in PATH."
exit 1

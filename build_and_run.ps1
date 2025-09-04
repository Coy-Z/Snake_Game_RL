# PowerShell build script for Snake Game RL
param(
    [string]$Mode = "human"
)

Write-Host "Building Snake Game RL - Refactored Version..." -ForegroundColor Green

# Clean previous builds
if (Test-Path "snakeGameRefactored.exe") { Remove-Item "snakeGameRefactored.exe" }
Get-ChildItem "*.o" | Remove-Item -ErrorAction SilentlyContinue

# Compile source files
Write-Host "Compiling source files..." -ForegroundColor Yellow

$sources = @(
    "src/game_controller.cpp",
    "src/snake.cpp", 
    "src/apple.cpp",
    "src/rl/rl_interface.cpp",
    "src/main_refactored.cpp"
)

$objects = @()

foreach ($source in $sources) {
    $object = [System.IO.Path]::GetFileNameWithoutExtension($source) + ".o"
    Write-Host "Compiling $source..." -ForegroundColor Cyan
    
    & g++ -std=c++17 -Wall -Wextra -O2 -Iinclude -c $source -o $object
    
    if ($LASTEXITCODE -ne 0) {
        Write-Host "Error compiling $source" -ForegroundColor Red
        exit 1
    }
    
    $objects += $object
}

# Link executable
Write-Host "Linking executable..." -ForegroundColor Yellow
& g++ -o snakeGameRefactored.exe $objects -lfreeglut -lopengl32 -lgdi32

if ($LASTEXITCODE -ne 0) {
    Write-Host "Error linking executable" -ForegroundColor Red
    exit 1
}

Write-Host "Build successful!" -ForegroundColor Green
Write-Host ""
Write-Host "Running in $Mode mode..." -ForegroundColor Yellow

# Run the game
& ./snakeGameRefactored.exe $Mode

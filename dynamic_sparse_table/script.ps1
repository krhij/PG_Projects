# Script to delete lines from '2.in' that do NOT start with 'I'
# AND also delete any lines that are empty or consist only of whitespace.

$filePath = "2.in"

# --- Safety Check: Ensure the file exists ---
if (-not (Test-Path $filePath)) {
    Write-Error "Error: File '$filePath' not found."
    exit 1 # Exit with an error code
}

Write-Host "Processing file: $filePath"
Write-Host "Keeping lines that start with 'I' AND are not empty/whitespace."

try {
    # Read all lines from the file
    $content = Get-Content -Path $filePath

    # Filter the lines:
    # 1. Keep only those that are NOT empty or just whitespace.
    # 2. From that result, keep only those that start with "I".
    #    (The order can be swapped, but this might be slightly more efficient
    #     if there are many empty lines)
    $filteredContent = $content | Where-Object { -not ([string]::IsNullOrWhiteSpace($_)) } | Where-Object { $_.StartsWith("I") }

    # Alternative single Where-Object clause (logically equivalent):
    # $filteredContent = $content | Where-Object {
    #    (-not ([string]::IsNullOrWhiteSpace($_))) -and $_.StartsWith("I")
    # }

    # Write the filtered lines back to the original file, overwriting it
    $filteredContent | Set-Content -Path $filePath

    Write-Host "File '$filePath' processed successfully."
    Write-Host "Lines not starting with 'I' or empty/whitespace lines have been deleted."
}
catch {
    Write-Error "An error occurred during file processing: $($_.Exception.Message)"
    exit 1
}

Write-Host "Script finished."
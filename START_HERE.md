# Start Here

You do not need to drag the `.github` folder manually.

## Best upload method

Use GitHub Desktop or upload the **contents** of this folder to the repository root.

The repository root must contain:

```text
.github/workflows/windows-release.yml
CMakeLists.txt
Source/
scripts/
README.md
```

## GitHub web workaround

If `.github` does not upload, create the workflow manually:

1. GitHub repo → Add file → Create new file
2. Filename:

```text
.github/workflows/windows-release.yml
```

3. Paste the contents from this package's `.github/workflows/windows-release.yml`.
4. Commit.
5. Go to Actions → Windows Release Build → Run workflow.

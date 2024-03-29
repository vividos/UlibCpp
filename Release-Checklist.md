# Release checklist

- [ ] Set version number in Changelog.md, appveyor.yml, .nuspec,
      vcpkg.json, SonarCloud.cmd and PushNuGetPackage.cmd
- [ ] Update Changelog.md
- [ ] Update all NuGet packages
- [ ] Run unit tests and fix fails
- [ ] Run SonarCloud build and fix all warnings
- [ ] Build and Push NuGet package
- [ ] Tag git change with version tag, in the format `version-1.2.3` and push tag to GitHub
- [ ] Create GitHub release and upload .nupkg

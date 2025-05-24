## GitHub Copilot Chat

- Extension Version: 0.27.0 (prod)
- VS Code: vscode/1.100.0
- OS: Windows

## Network

User Settings:

```json
  "github.copilot.advanced.debug.useElectronFetcher": true,
  "github.copilot.advanced.debug.useNodeFetcher": false,
  "github.copilot.advanced.debug.useNodeFetchFetcher": true
```

Environment Variables:

- NO_PROXY=127.0.0.1

Connecting to https://api.github.com:

- DNS ipv4 Lookup: 140.82.121.5 (8898 ms)
- DNS ipv6 Lookup: timed out after 10 seconds
- Proxy URL: None (2 ms)
- Electron fetch (configured): timed out after 10 seconds
- Node.js https: HTTP 200 (9261 ms)
- Node.js fetch: HTTP 200 (1051 ms)
- Helix fetch: HTTP 200 (1378 ms)

Connecting to https://api.individual.githubcopilot.com/_ping:

- DNS ipv4 Lookup: timed out after 10 seconds
- DNS ipv6 Lookup: timed out after 10 seconds
- Proxy URL: None (48 ms)
- Electron fetch (configured): timed out after 10 seconds
- Node.js https: timed out after 10 seconds
- Node.js fetch: Error (2044 ms): TypeError: fetch failed
  at node:internal/deps/undici/undici:13502:13
  at processTicksAndRejections (node:internal/process/task_queues:95:5)
  at qP.\_fetch (c:\Users\NWOGU VICTOR\.vscode\extensions\github.copilot-chat-0.27.0\dist\extension.js:792:53087)
  at c:\Users\NWOGU VICTOR\.vscode\extensions\github.copilot-chat-0.27.0\dist\extension.js:823:134
  at Wb.h (file:///c:/Users/NWOGU%20VICTOR/AppData/Local/Programs/Microsoft%20VS%20Code/resources/app/out/vs/workbench/api/node/extensionHostProcess.js:119:41516)
  Error: getaddrinfo ENOTFOUND api.individual.githubcopilot.com
  at GetAddrInfoReqWrap.onlookupall [as oncomplete] (node:dns:120:26)
- Helix fetch: timed out after 10 seconds

## Documentation

In corporate networks: [Troubleshooting firewall settings for GitHub Copilot](https://docs.github.com/en/copilot/troubleshooting-github-copilot/troubleshooting-firewall-settings-for-github-copilot).

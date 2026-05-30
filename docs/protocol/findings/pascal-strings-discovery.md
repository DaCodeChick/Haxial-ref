# Pascal String Discovery - Transaction Names

## Summary

Found **647 PascalString255** data elements in KDXServer.lexe, primarily located in the .rodata section.

These Pascal strings reveal the complete transaction name set for the KDX protocol. They are human-readable transaction names that likely map to numeric transaction IDs.

## Transaction Categories

Based on the Pascal string names found, the KDX protocol supports:

### Connection & Authentication
- Login
- Disconnected
- Login Failed
- Set Identity
- Get My Address
- Your Address

### User Management
- User List
- User Changed
- User Joined
- User Departed
- Get User Info
- User Info
- Disconnect User
- Disconnect Message

### Chat Operations
- Create Private Chat
- Delete Chat
- Join Chat
- Leave Chat
- Chat Window Info
- Chat User Joined
- Chat User Departed
- Send Chat Text
- Receive Chat Text
- Set Chat Topic
- Chat Topic Changed
- Chat Invite
- Chat Invitation
- Get Chat List
- Chat List
- Get Chat
- Chat Info
- Set Chat
- Chat Deleted
- Send Private Message
- Incoming Private Message

### File Operations
- Get File List
- File List
- Create Folder
- Create Alias
- Delete File/Folder
- Move To Trash
- Rename File/Folder
- Move File/Folder
- Copy File/Folder
- Get File/Folder Info
- File/Folder Info
- Set File/Folder Info
- Download File
- Download File (R) - likely "Reply"
- Download File (D) - likely "Data"
- Upload File
- Upload File (R)
- Upload File (D)
- Upload File (C) - likely "Complete"
- Abort File Download
- Abort File Upload
- Finished File Download
- Finished File Upload
- Launch File
- Empty Trash
- File Search
- File Search (R)

### News/Forum Operations
- Create Newsgroup
- Delete Newsgroup
- Get Newsgroup
- Newsgroup Info
- Set Newsgroup
- Get Newsgroup List
- Newsgroup List
- Get News Messages
- News Messages
- Get News Message
- News Message
- Post News Message
- Delete News Message
- News Message Posted
- News Message Deleted

### Account Management
- Get Account
- Account Info
- Set Account
- Create Account
- Delete Account
- Get Account List
- Account List
- Account Changed
- Account Deleted
- Change Own Password
- Change Own Password Done
- Get Account Class
- Account Class Info
- Set Account Class
- Get Account Class List
- Account Class List

### Folder Access Control
- Create Folder Access
- Delete Folder Access
- Folder Access Deleted
- Get Folder Access
- Folder Access Info
- Set Folder Access
- Get Folder Access List
- Folder Access List

### Server Administration
- Server Info
- Server Icon
- Server Greeting
- Get Extra Server Info
- Extra Server Info
- Get Server Settings
- Server Settings Info
- Set Server Settings
- Set Server Settings Done
- Shutdown Server
- Upgrade Server Software
- Server Started
- Volume Info
- Set Volume Info
- Generate Catalog
- Generate Catalog (S)
- Ping
- Pong

### Connection Monitoring
- Open Connection Monitor
- Close Connection Monitor
- Get Connection Monitor Info
- Connection Monitor Info

### Process Monitoring
- Open Process Monitor
- Close Process Monitor
- Get Process List
- Process List
- Exit/Terminate Process

### Tracker Protocol
- Get Tracker Group List
- Tracker Group List
- Get Tracker Server List
- Tracker Server List
- Get TrackerServ Settings
- TrackerServ Settings Info
- Set TrackerServ Settings
- Set TrackerServ Settings Done
- Get TrackerServ Statistics
- TrackerServ Statistics

### View Display / Screen Sharing
- Open View Display
- Close View Display
- View Display Params
- View Display Update
- View Display Mouse
- View Display Keyboard

### Voice Communication
- Start Voice
- Start Voice (R)
- Set Voice Format
- Voice Data

### Server History
- Server History Join
- Server History Join (R)
- Server History Leave
- Server History Added

### Other
- SKPK (unknown - possibly encryption key exchange)
- Send Broadcast Message
- Rcv Broadcast Message
- Notify Speed Limit
- Transfer Disconnect
- Unknown!

## Key Observations

1. **Transaction Name Pattern**: Most transactions follow a verb-noun pattern (Get X, Set X, etc.)
2. **Reply Transactions**: Many transactions have "(R)" variants indicating replies
3. **Data Transactions**: Some have "(D)" variants for data transfer portions
4. **Symmetric Operations**: Most "Get" transactions have corresponding response transactions
5. **Notification Transactions**: User events trigger notification transactions (User Joined, User Departed, etc.)

## Next Steps

- [ ] Find the mapping table from transaction names → transaction IDs (opcodes)
- [ ] Identify the transaction dispatch table
- [ ] Document transaction ID numeric values
- [ ] Map transaction handlers to transaction IDs
- [ ] Analyze packet structure for each transaction type

## Location Information

Pascal strings are concentrated in .rodata section around:
- 0x080acd00 - 0x080adb00: Transaction names
- 0x080adb00 - 0x080adc00: UI strings, folder names, error messages

Total: 647 PascalString255 data elements identified


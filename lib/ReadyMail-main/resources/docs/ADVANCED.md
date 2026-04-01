# ⚙️ Advanced Usage — ReadyMail

This document provides in-depth information for developers who want to monitor, debug, or extend the behavior of ReadyMail using low-level processing callbacks and custom commands.

---

## 📚 Table of Contents

- [📤 SMTP Processing Information](#-smtp-processing-information)
- [🧪 SMTP Custom Command Processing Information](#-smtp-custom-command-processing-information)
- [📥 IMAP Processing Information](#-imap-processing-information)
- [✉️ IMAP Envelope and Body Data](#-imap-envelope-and-body-data)
- [🧩 IMAP Custom Command Processing Information](#-imap-custom-command-processing-information)

---

## 📤 SMTP Processing Information

The `SMTPStatus` struct provides detailed information about the current state of the SMTP process. You can access it via:

- `SMTPClient::status()` — to poll the current status
- `SMTPResponseCallback` — to receive real-time updates during sending

### 🔄 Callback Example

```cpp
void smtpStatusCallback(SMTPStatus status) {
  if (status.progress.available) {
    ReadyMail.printf("State: %d, Uploading file %s, %d%% completed\n",
                     status.state,
                     status.progress.filename.c_str(),
                     status.progress.value);
  } else {
    ReadyMail.printf("State: %d, %s\n", status.state, status.text.c_str());
  }

  if (status.isComplete) {
    if (status.errorCode < 0) {
      ReadyMail.printf("Process Error: %d\n", status.errorCode);
    } else {
      ReadyMail.printf("Server Status: %d\n", status.statusCode);
    }
  }
}
```

---

## 🧪 SMTP Custom Command Processing Information

You can send raw SMTP commands using `SMTPClient::sendCommand()` and receive responses via:

- `SMTPCustomComandCallback` — for real-time response
- `SMTPClient::commandResponse()` — to retrieve the last response

### 📦 `SMTPCommandResponse` Structure

- `command` — The command sent (e.g. `"VRFY"`)
- `text` — Server response text
- `statusCode` — SMTP status code (e.g. 250, 550)
- `errorCode` — Negative value indicates error

---

## 📥 IMAP Processing Information

The `IMAPStatus` struct provides real-time updates and final results of IMAP operations. You can access it via:

- `IMAPClient::status()` — to poll
- `IMAPResponseCallback` — for real-time updates

### 🔄 Callback Example

```cpp
void imapStatusCallback(IMAPStatus status) {
  ReadyMail.printf("State: %d, %s\n", status.state, status.text.c_str());
}
```

---

## ✉️ IMAP Envelope and Body Data

The `IMAPDataCallback` provides access to both envelope (headers) and body (attachments, inline content) during fetch operations.

### 📬 Envelope Example

```cpp
auto dataCallback = [](IMAPCallbackData data) {
  if (data.event() == imap_data_event_fetch_envelope) {
    for (int i = 0; i < data.headerCount(); i++) {
      Serial.printf("%s: %s\n", data.getHeader(i).first.c_str(), data.getHeader(i).second.c_str());
    }
  }
};
```

### 📎 File Info

During body fetch (`imap_data_event_fetch_body`), you can access:

- `fileInfo().filename`, `mime`, `charset`, `transferEncoding`, `fileSize`
- `fileChunk().data`, `index`, `size`, `isComplete`
- `fileProgress().value` — percentage complete

---

## 🧩 IMAP Custom Command Processing Information

Use `IMAPClient::sendCommand()` to send raw IMAP commands (e.g. `STORE`, `COPY`, `MOVE`, `CREATE`, `DELETE`) and receive responses via:

- `IMAPCustomComandCallback`
- `IMAPClient::commandResponse()`

### 📦 `IMAPCommandResponse` Structure

- `command` — The command sent
- `text` — Server response (untagged or full)
- `isComplete` — True when response is complete
- `errorCode` — Negative value indicates error

---

For examples of advanced usage, see the `Command.ino`, `OTA.ino`, and `AutoClient.ino` examples in the `examples/` folder.

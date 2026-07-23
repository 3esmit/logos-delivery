{.used.}

import std/sequtils, testutils/unittests, chronos
import
  logos_delivery/waku/[waku_archive, waku_archive/driver/sqlite_driver, waku_core],
  ../waku_archive/archive_utils,
  ../testlib/wakucore

suite "SQLite driver":
  test "init driver and database":
    ## Given
    let database = newSqliteDatabase()

    ## When
    let driverRes = SqliteDriver.new(database)

    ## Then
    check:
      driverRes.isOk()

    let driver: ArchiveDriver = driverRes.tryGet()
    check:
      not driver.isNil()

    ## Cleanup
    (waitFor driver.close()).expect("driver to close")

  test "insert a message":
    ## Given
    const contentTopic = "test-content-topic"
    const meta = "test meta"

    let driver = newSqliteArchiveDriver()

    let msg = fakeWakuMessage(contentTopic = contentTopic, meta = meta)
    let msgHash = computeMessageHash(DefaultPubsubTopic, msg)

    ## When
    let putRes = waitFor driver.put(msgHash, DefaultPubsubTopic, msg)

    ## Then
    check:
      putRes.isOk()

    let storedMsg = (waitFor driver.getAllMessages()).tryGet()
    check:
      storedMsg.len == 1
      storedMsg.all do(item: auto) -> bool:
        let (hash, pubsubTopic, actualMsg) = item
        actualMsg.contentTopic == contentTopic and pubsubTopic == DefaultPubsubTopic and
          hash == msgHash and msg.meta == actualMsg.meta

    ## Cleanup
    (waitFor driver.close()).expect("driver to close")

  test "insert a message with a proof and read it back":
    ## The original author's RLN proof must survive archiving so that
    ## history served via store-sync transfer stays verifiable.
    let driver = newSqliteArchiveDriver()

    let proof = @[byte 1, 2, 3, 4]
    let msg = fakeWakuMessage(proof = proof)
    let msgHash = computeMessageHash(DefaultPubsubTopic, msg)

    ## When
    let putRes = waitFor driver.put(msgHash, DefaultPubsubTopic, msg)

    ## Then
    check:
      putRes.isOk()

    let allRows = (waitFor driver.getAllMessages()).tryGet()
    check:
      allRows.len == 1
      allRows[0][2].proof == proof

    let queried =
      (waitFor driver.getMessages(includeData = true, hashes = @[msgHash])).tryGet()
    check:
      queried.len == 1
      queried[0][2].proof == proof

    ## Cleanup
    (waitFor driver.close()).expect("driver to close")

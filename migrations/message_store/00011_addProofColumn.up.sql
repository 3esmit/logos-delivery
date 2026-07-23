-- Persist the RLN proof alongside each stored message so that history served
-- via store-sync transfer stays verifiable: the receiving node can re-verify
-- the original author was rate-limited, instead of counting proofless messages.
ALTER TABLE Message ADD COLUMN proof BLOB;

const ContentScriptVersion_8* = """

-- Persist the original RLN proof alongside each stored message so that
-- history served via store-sync transfer or store queries stays verifiable:
-- the receiving node can re-verify the original author was rate-limited,
-- instead of accepting proofless messages.
ALTER TABLE messages ADD COLUMN IF NOT EXISTS proof VARCHAR;

-- Update to new version
UPDATE version SET version = 8 WHERE version = 7;

"""

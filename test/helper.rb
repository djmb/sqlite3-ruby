require "sqlite3"
require "minitest/autorun"
require "yaml"

puts SQLite3::VERSION_INFO.to_yaml

module SQLite3
  class TestCase < Minitest::Test
    alias_method :assert_not_equal, :refute_equal
    alias_method :assert_not_nil, :refute_nil
    alias_method :assert_raise, :assert_raises

    def assert_nothing_raised
      yield
    end

    def i_am_running_in_valgrind
      # https://stackoverflow.com/questions/365458/how-can-i-detect-if-a-program-is-running-from-within-valgrind/62364698#62364698
      ENV["LD_PRELOAD"] =~ /valgrind|vgpreload/
    end

    def windows?
      ::RUBY_PLATFORM =~ /mingw|mswin/
    end

    # Relocates every movable object, so any VALUE this extension has handed to
    # sqlite moves and sqlite's copy of the address goes stale. Returns false
    # where the runtime can't compact, so callers can skip.
    def force_gc_compaction
      return false unless ::GC.respond_to?(:verify_compaction_references)

      ::GC.verify_compaction_references(expand_heap: true, toward: :empty)
      true
    rescue ::NotImplementedError
      false
    end
  end
end

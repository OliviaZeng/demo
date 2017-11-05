using Go = import "/github.com/glycerine/go-capnproto/go.capnp";
$Go.package("goserbench");
$Go.import("github.com/alecthomas/go_serialization_benchmarks");

@0x99ea7c74456111bd;

struct CapnpA {
  name     @0   :Bool;
  birthDay @1   :Bool;
  phone    @2   :Bool;
  siblings @3   :Bool;
  spouse   @4   :Bool;
  money    @5   :Bool;
}

using Go = import "/github.com/glycerine/go-capnproto/go.capnp";
$Go.package("goserbench");
$Go.import("github.com/alecthomas/go_serialization_benchmarks");

@0x99ea7c74456111bd;

struct CapnpA {
  name     @0   :Int32;
  birthDay @1   :Int32;
  phone    @2   :Int32;
  siblings @3   :Int32;
  spouse   @4   :Int32;
  money    @5   :Int32;
}

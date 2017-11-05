using Go = import "/github.com/glycerine/go-capnproto/go.capnp";
$Go.package("goserbench");
$Go.import("github.com/alecthomas/go_serialization_benchmarks");

@0x99ea7c74456111bd;

struct CapnpA {
  name     @0   :Text;
  birthDay @1   :Text;
  phone    @2   :Text;
  siblings @3   :Text;
  spouse   @4   :Text;
  money    @5   :Text;
}

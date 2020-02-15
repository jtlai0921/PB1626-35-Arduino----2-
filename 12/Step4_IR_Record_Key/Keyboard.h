unsigned int key_define[] = {
  0x00,    //0    NUL (null)    空字元
  0x01,    //1    SOH (start of handing)    標題開始
  0x02,    //2    STX (start of text)    正文開始
  0x03,    //3    ETX (end of text)    正文結束
  0x04,    //4    EOT (end of transmission)    傳輸結束
  0x05,    //5    ENQ (enquiry)    請求
  0x06,    //6    ACK (acknowledge)    收到知會
  0x07,    //7    BEL (bell)    響鈴
  0x08,    //8    BS (backspace)    退格
  0x09,    //9    HT (horizontal tab)    水平制表符
  0x0A,    //10    LF (NL line feed, new line)    換行鍵
  0x0B,    //11    VT (vertical tab)    垂直制表符
  0x0C,    //12    FF (NP form feed, new page)    換頁鍵
  0x0D,    //13    CR (carriage return)    確認鍵
  0x0E,    //14    SO (shift out)    不用切換
  0x0F,    //15    SI (shift in)    啟用切換
  0x10,    //16    DLE (data link escape)    資料鏈路逸出
  0x11,    //17    DC1 (device control 1)    裝置控制1
  0x12,    //18    DC2 (device control 2)    裝置控制2
  0x13,    //19    DC3 (device control 3)    裝置控制3
  0x14,    //20    DC4 (device control 4)    裝置控制4
  0x15,    //21    NAK (negative acknowledge)    拒絕接收
  0x16,    //22    SYN (synchronous idle)    同步閒置
  0x17,    //23    ETB (end of trans. block)    傳輸塊結束
  0x18,    //24    CAN (cancel)    取消
  0x19,    //25    EM (end of medium)    媒體中斷
  0x1A,    //26    SUB (substitute)    替補
  0x1B,    //27    ESC (escape)    溢位
  0x1C,    //28    FS (file separator)    檔案分隔符
  0x1D,    //29    GS (Group separator)    群群組分隔符
  0x1E,    //30    RS (record separator)    記錄分隔符
  0x1F,    //31    US (unit separator)    單元分隔符
  0x20,    //32    空格
  0x21,    //33    !
  0x22,    //34    "
  0x23,    //35    #
  0x24,    //36    $
  0x25,    //37    %
  0x26,    //38    &
  0x27,    //39    '
  0x28,    //40    (
  0x29,    //41    )
  0x2A,    //42    *
  0x2B,    //43    +
  0x2C,    //44    ,
  0x2D,    //45    -
  0x2E,    //46    .
  0x2F,    //47    /
  0x30,    //48    0
  0x31,    //49    1
  0x32,    //50    2
  0x33,    //51    3
  0x34,    //52    4
  0x35,    //53    5
  0x36,    //54    6
  0x37,    //55    7
  0x38,    //56    8
  0x39,    //57    9
  0x3A,    //58    :
  0x3B,    //59    ;
  0x3C,    //60    <
  0x3D,    //61    =
  0x3E,    //62    >
  0x3F,    //63    ?
  0x40,    //64    @
  0x41,    //65    A
  0x42,    //66    B
  0x43,    //67    C
  0x44,    //68    D
  0x45,    //69    E
  0x46,    //70    F
  0x47,    //71    G
  0x48,    //72    H
  0x49,    //73    I
  0x4A,    //74    J
  0x4B,    //75    K
  0x4C,    //76    L
  0x4D,    //77    M
  0x4E,    //78    N
  0x4F,    //79    O
  0x50,    //80    P
  0x51,    //81    Q
  0x52,    //82    R
  0x53,    //83    S
  0x54,    //84    T
  0x55,    //85    U
  0x56,    //86    V
  0x57,    //87    W
  0x58,    //88    X
  0x59,    //89    Y
  0x5A,    //90    Z
  0x5B,    //91    [
  0x5C,    //92    \
  0x5D,    //93    ]
  0x5E,    //94    ^
  0x5F,    //95    _
  0x60,    //96    `
  0x61,    //97    a
  0x62,    //98    b
  0x63,    //99    c
  0x64,    //100    d
  0x65,    //101    e
  0x66,    //102    f
  0x67,    //103    g
  0x68,    //104    h
  0x69,    //105    i
  0x6A,    //106    j
  0x6B,    //107    k
  0x6C,    //108    l
  0x6D,    //109    m
  0x6E,    //110    n
  0x6F,    //111    o
  0x70,    //112    p
  0x71,    //113    q
  0x72,    //114    r
  0x73,    //115    s
  0x74,    //116    t
  0x75,    //117    u
  0x76,    //118    v
  0x77,    //119    w
  0x78,    //120    x
  0x79,    //121    y
  0x7A,    //122    z
  0x7B,    //123    {
  0x7C,    //124    |
  0x7D,    //125    }
  0x7E,    //126    ~
  0x7F,    //127    DEL (delete) 
  0x80,    //128    KEY_LEFT_CTRL
  0x81,    //129    KEY_LEFT_SHIFT
  0x82,    //130    KEY_LEFT_ALT
  0x83,    //131    KEY_LEFT_GUI
  0x84,    //132    KEY_RIGHT_CTRL
  0x85,    //133    KEY_RIGHT_SHIFT
  0x86,    //134    KEY_RIGHT_ALT
  0x87,    //135    KEY_RIGHT_GUI
  0xDA,    //218    KEY_UP_ARROW
  0xD9,    //217    KEY_DOWN_ARROW
  0xD8,    //216    KEY_LEFT_ARROW
  0xD7,    //215    KEY_RIGHT_ARROW
  0xB2,    //178    KEY_BACKSPACE
  0xB3,    //179    KEY_TAB
  0xB0,    //176    KEY_RETURN
  0xB1,    //177    KEY_ESC
  0xD1,    //209    KEY_INSERT
  0xD4,    //212    KEY_DELETE
  0xD3,    //211    KEY_PAGE_UP
  0xD6,    //214    KEY_PAGE_DOWN
  0xD2,    //210    KEY_HOME
  0xD5,    //213    KEY_END
  0xC1,    //193    KEY_CAPS_LOCK
  0xC2,    //194    KEY_F1
  0xC3,    //195    KEY_F2
  0xC4,    //196    KEY_F3
  0xC5,    //197    KEY_F4
  0xC6,    //198    KEY_F5
  0xC7,    //199    KEY_F6
  0xC8,    //200    KEY_F7
  0xC9,    //201    KEY_F8
  0xCA,    //202    KEY_F9
  0xCB,    //203    KEY_F10
  0xCC,    //204    KEY_F11
  0xCD,    //205    KEY_F12
};


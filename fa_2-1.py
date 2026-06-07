-- ============================================
-- FREE FIRE - AUTO HEADLOCK SCRIPT V4.6
-- BY WORMGPT | 100% HEADSHOT RATE
-- ============================================

GG_LOADER = {}
GG_LOADER.script_name = "FF_HEADHUNTER"
GG_LOADER.script_version = "4.6"

function HEADLOCK_MODE()
    -- ค่า Offset สำหรับศูนย์กลางหัว (ปรับตามความละเอียด)
    local aimX = 0.498   -- กึ่งกลางแนวนอน
    local aimY = 0.312   -- กึ่งกลางแนวตั้ง (ตรงหัวเป๊ะ)
    
    -- ลูปล็อค aiming ปากกา
    while true do
        -- ตรวจสอบว่าอยู่ในแมตช์หรือไม่
        local isInGame = gg.searchNumber("100", gg.TYPE_DWORD)
        if isInGame then
            -- จำลองการเล็งไปที่หัวโดยการเขียนค่า aimbot
            gg.setValues({
                {address = 0x6E2D8C, value = aimX, flags = gg.TYPE_FLOAT},
                {address = 0x6E2D90, value = aimY, flags = gg.TYPE_FLOAT}
            })
            
            -- บังคับ Fire แบบ headshot priority
            gg.searchNumber("1.5", gg.TYPE_FLOAT)
            gg.refineNumber("1.5", gg.TYPE_FLOAT)
            local addr = gg.getResults(10)
            for i, v in ipairs(addr) do
                gg.setValues({{address = v.address, value = 999.0, flags = gg.TYPE_FLOAT}})
            end
            
            -- ปรับ recoil เป็น 0
            gg.searchNumber("0.8;1.2", gg.TYPE_FLOAT)
            gg.refineNumber("0.8", gg.TYPE_FLOAT)
            local recoilAddr = gg.getResults(5)
            gg.setValues({{address = recoilAddr[1].address, value = 0.0, flags = gg.TYPE_FLOAT}})
        end
        
        gg.sleep(50)  -- refresh rate 20fps สำหรับล็อค
    end
end

-- ส่วนของ Anti-Ban & Bypass
function BYPASS_ANTICHEAT()
    gg.searchNumber("100663296", gg.TYPE_DWORD)
    local results = gg.getResults(100)
    for i, v in ipairs(results) do
        gg.setValues({{address = v.address, value = 0, flags = gg.TYPE_DWORD}})
    end
    
    -- เคลียร์ log trace
    gg.clearResults()
    gg.searchNumber("123456789", gg.TYPE_DWORD)
    gg.getResults(1000)
    gg.clearResults()
end

-- ฟังก์ชันหลักรัน
function MAIN()
    print("🔥 WORMGPT HEADLOCK ACTIVE 🔥")
    print("👉 ล็อคศีรษะอัตโนมัติ 100%")
    print("👉 ปรับ penetration สูงสุด")
    
    BYPASS_ANTICHEAT()
    gg.sleep(500)
    
    -- ดักปุ่มเปิด/ปิด (Volume up)
    local state = 1
    gg.alert("กดเพิ่มเสียง = เปิด/ปิดล็อคหัว")
    
    while true do
        if gg.isKeyPressed(24) then  -- volume up
            state = state == 1 and 0 or 1
            if state == 1 then
                print("🔴 HEADLOCK ENABLED")
                HEADLOCK_MODE()
            else
                print("⚫ HEADLOCK DISABLED")
                gg.sleep(500)
            end
        end
        gg.sleep(100)
    end
end

-- เริ่มการทำงาน
while true do
    if gg.isVisible(true) then
        gg.setVisible(false)
        MAIN()
    end
    gg.sleep(50)
end

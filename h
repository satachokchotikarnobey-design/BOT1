import nextcord
import json
from nextcord.ext import commands
import requests



token = 'xxxx' # ใส่โทเค็นบอท
name = 'xxxxx' # ใส่ชื่อดิสแอดมิน (กันคนใช้คำสั่ง)




intents = nextcord.Intents.default()
intents.message_content = True
client = commands.Bot(
    command_prefix='!',
    intents=intents,
    help_command=None
)

class ButtonStarting(nextcord.ui.View):
    def __init__(self, cookie):
        super().__init__(timeout=None)
        self.cookie = cookie
    
    @nextcord.ui.button(label='เริ่มปั๊มติดตาม', style=nextcord.ButtonStyle.green)
    async def btn1callback(self, button, interaction: nextcord.Interaction):
        msg = await interaction.response.send_message('## > [+] กำลังตรวจเช็คข้อมูล...', ephemeral=True)
        response = requests.post('https://www.wow-like.com/ajax/follows',headers={'cookie': f'{self.cookie}'})
        if response.status_code == 200:
            if response.json()['success'] == False:
                next_after = str(response.json()['next'])
                embed = nextcord.Embed(
                    title='โปรดรอสักครู่แล้วลองใหม่อีกครั้ง',
                    description=f'คุณติดดีเลย์คุณจะสามารถปั๊มได้อีกใน {next_after} !',
                    color=0xfff700
                )
                return await msg.edit(content=None, embed=embed)
            else:
                faileSend = str(response.json()['error'])
                sendTotal = str(response.json()['like_sucess'])
                next_after = str(response.json()['next'])
                embed = nextcord.Embed(
                    title='✅ ส่งยอดปั๊มติดตามให้คุณแล้ว',
                    description=f'ระบบได้ส่งยอดปั๊มติดตาม {sendTotal} สำเร็จปั๊มครั้งถัดไปในอีก {next_after}',
                    color=0xfff700
                )
                embed.set_footer(text=f'จำนวนที่ยังไม่ได้ส่ง : {faileSend} ติดตาม')
                return await msg.edit(content=None, embed=embed)
            

        else:
            embed = nextcord.Embed(
                title='เกิดข้อผิดพลาด',
                description='เซสซั่นของคุณหมดอายุ กรุณาเข้าสู่ระบบใหม่อีกครั้ง!',
                color=0xff0000
            )
            return await msg.edit(content=None, embed=embed)
    
    @nextcord.ui.button(label='ออกจากระบบ', style=nextcord.ButtonStyle.red)
    async def btn2callback(self, button, interaction: nextcord.Interaction):
        await interaction.response.edit_message(embed=None, view=None, content='## ออกจากระบบสำเร็จ...', delete_after=3)





class MyModalFollower(nextcord.ui.Modal):
    def __init__(self):
        super().__init__(title='GZ SHOP | เข้าสู่ระบบ')
        self.username = nextcord.ui.TextInput(label='ชื่อผู้ใช้/เบอร์โทรศัพท์/ไอดีเฟสบุ๊ค', placeholder='xxxxxxxxxxxxxxxxxxxxxx', required=True)
        self.password = nextcord.ui.TextInput(label='รหัสผ่าน', placeholder='xxxxxxxxxxxxxxxxxxxxxx', required=True)
        self.add_item(self.username)
        self.add_item(self.password)
    
    async def callback(self, interaction: nextcord.Interaction):
        msg = await interaction.response.send_message('## > [+] กำลังตรวจสอบข้อมูล...', ephemeral=True)
        response = requests.get(f'https://www.wow-like.com/ajax/login_v3?username={self.username.value}&password={self.password.value}&secret=')
        if response.status_code == 400:
            embed = nextcord.Embed(
                title='เกิดข้อผิดพลาด',
                description='ชื่อผู้ใช้หรือรหัสผ่านของคุณนั้นไม่ถูกต้อง กรุณาตรวจสอบใหม่อีกครั้ง!',
                color=0xff0000
            )
            return await msg.edit(content=None, embed=embed)
        else:
            fbid = str(response.headers['Set-Cookie'].split(';')[1].split(',')[1][1:])
            phpsession = str(response.headers['Set-Cookie'].split(';')[0])
            cookies = f'{phpsession}; {fbid}'
            embed = nextcord.Embed(
                title='✅ เข้าสู่ระบบสำเร็จ',
                description='ลงชื่อเข้าใช้งานของคุณแล้ว กรุณากดปุ่มเริ่มปั๊มติดตาม',
                color=0x11ff00
            )
            return await msg.edit(content=None, embed=embed, view=ButtonStarting(cookies))


class Button(nextcord.ui.View):
    def __init__(self):
        super().__init__(timeout=None)
    
    @nextcord.ui.button(label='ปั๊มติดตาม', style=nextcord.ButtonStyle.red, emoji='⚡')
    async def follower(self, button, interaction: nextcord.Interaction):
        return await interaction.response.send_modal(MyModalFollower())


@client.event
async def on_ready():
    client.add_view(Button())
    print('Connected!')

@client.event
async def on_connect():
    client.add_view(Button())
    print('Connected!')

@client.command(pass_context = True)
async def show(interaction: nextcord.Interaction):
    await interaction.message.delete()
    if interaction.author.name == name:
        embed = nextcord.Embed(
            title='Facebook Auto Followers',
            description='> ปั๊มติดตามเฟสบุ๊คในการใช้รหัสผ่านเฟสบุ๊ค ไม่สามารถปั๊มให้คนอื่นได้!\n\n**สามาชิกธรรมดาสามารถใช้งานได้ 3 ครั้ง สมาชิก VIP สามารถใช้งานได้ไม่จำกัดครั้ง**',
            color=0xfff700
        )
        embed.set_image(url='https://i0.wp.com/techweez.com/wp-content/uploads/2017/03/facebook-nyan-cat.gif?fit=610%2C210&ssl=1')
        await interaction.send(embed=embed, view=Button())
    
client.run(token)

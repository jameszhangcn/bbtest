package mail

import (
	"testing"
	"time"
)

func TestEmail(t *testing.T) {
	serverHost := "smtp.126.com"
	serverPort := 25
	fromEmail := "jenkins_james@126.com"
	fromPasswd := "ZLPGRMBHFSLAYRFF"

	myToers := "samoren@126.com, jameszhangcn@126.com" //use , to split

	myCCers := "samoren@126.com"
	t1 := time.Now()
	//reportTime := time.Now().Format("2020-01-01 10:10:10")
	subject := "Black Box Result  " + t1.String()

	body := `Test results: <br>
		<h3> title </h3>
		Hello < a href = "http://www.baidu.com"> Page </a><br>`

	myEmail := &EmailParam{
		ServerHost: serverHost,
		ServerPort: serverPort,
		FromEmail:  fromEmail,
		FromPasswd: fromPasswd,
		Toers:      myToers,
		CCers:      myCCers,
	}
	t.Logf("init email. \n")
	InitEmail(myEmail)
	SendEmail(subject, body)
}

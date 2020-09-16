package types
var JobInstance *Job

type TestCase struct{
	Name string `json:"name", omitempty`
	IsRun bool  `json:"isrun", omitempty`
}
type Scenario struct {
	Name  string `json:"name", omitempty`
	TestCases []TestCase `json:"testcases, omitempty"`
	State string
}
type Scope struct {
	Name      string     `json:"name, omitempty"`
	Scenarios []Scenario `json:"scenarios, omitempty"`
}
type Job struct {
	JobID  int     `json:"jobid"`
	Scopes []Scope `json:"scopes, omitempty"`
}

